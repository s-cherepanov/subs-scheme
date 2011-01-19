/*
 *  Subs Scheme Interpreter
 *  Copyright (C) 2010 Andy Balaam
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**/

#include <iosfwd>
#include <memory>
#include <string>

#include "lib/value/basic/combinationvalue.h"
#include "lib/value/symbol/defineutilities.h"
#include "lib/value/symbol/lambdasymbolvalue.h"
#include "lib/value/symbol/lambdautilities.h"
#include "lib/value/symbol/letsymbolvalue.h"
#include "lib/value/value.h"
#include "lib/environment.h"
#include "lib/evaluationcontext.h"
#include "lib/evaluationerror.h"
#include "lib/evaluator.h"
#include "lib/prettyprinter.h"
#include "lib/specialsymbolevaluator.h"

using namespace std;

namespace
{

class SymbolValuePairArray
{
public:
    SymbolValuePairArray( size_t size )
    : size_( size )
    , symbols_array_( new string[size] )
    , values_array_( new auto_ptr<Value>[size] )
    {
    }

    ~SymbolValuePairArray()
    {
        delete[] symbols_array_;
        delete[] values_array_;
    }

    void Insert( size_t index, const string& symbol,
        auto_ptr<Value> value )
    {
        assert( index < size_ );
        symbols_array_[index] = symbol;
        values_array_[index] = value;
    }

    const string& GetSymbol( size_t index ) const
    {
        assert( index < size_ );
        return symbols_array_[index];
    }

    auto_ptr<Value>& GetValue( size_t index )
    {
        assert( index < size_ );
        return values_array_[index];
    }

    size_t size()
    {
        return size_;
    }

private:
    // Ensure no copy constructor since we own auto_ptrs
    SymbolValuePairArray( const SymbolValuePairArray& other );

    size_t size_;
    string* symbols_array_;
    auto_ptr<Value>* values_array_;
};

/**
 * Go through all the symbol-value pairs, evaluating the
 * values remembering them along with the symbol names.
 * Don't insert them into the environment yet since we
 * don't want the definitions to interfere with each other.
 */
void evaluate_symbol_values( const CombinationValue* pairs,
    EvaluationContext& ev, const CombinationValue* combo,
    SymbolValuePairArray& retsymvalpairs )
{
    size_t count = 0;
    for( CombinationValue::const_iterator itp = pairs->begin();
         itp != pairs->end();
         ++count,
         ++itp )
    {
        const CombinationValue* pair = dynamic_cast<const CombinationValue*>(
            *itp );
        if( !pair )
        {
            throw EvaluationError( "Each item in the first argument to "
                "the let operator must be a combination containing a "
                "symbol and a value.  '"
                + PrettyPrinter::Print( *itp )
                + "' is not a combination." );
        }

        if( pair->size() != 2 )
        {
            throw EvaluationError( "Each item in the first argument to "
                "the let operator must be a combination containing a "
                "symbol and a value.  '"
                + PrettyPrinter::Print( *itp )
                + "' does not consist of 2 parts." );
        }

        CombinationValue::const_iterator itinpair = pair->begin();
        assert( itinpair != pair->end() ); // First of 2
        const Value* namev = *itinpair;
        ++itinpair;
        assert( itinpair != pair->end() ); // Second of 2
        const Value* value = *itinpair;

        const SymbolValue* name = dynamic_cast<const SymbolValue*>( namev );
        if( !name )
        {
            throw EvaluationError( "Each item in the first argument to "
                "the let operator must be a combination containing a "
                "symbol and a value.  '"
                + PrettyPrinter::Print( namev )
                + "' is not a symbol." );
        }

        retsymvalpairs.Insert( count, name->GetStringValue(),
            ev.SubEval( value ) );
    }
}

/**
 * We have encountered a let whose return value is the return value of
 * the function we are in.  We can optimise by modelling this as defines
 * of the let variables, since this environment is disposable anyway,
 * so the leaking out of the defined values has no effect.
 */
const Value* process_let_tail_call( EvaluationContext& ev,
    const CombinationValue* combo )
{
    if( combo->size() < 3 )
    {
        throw EvaluationError(
            "Too few operands to the let operator: there should "
            "be at least 2: a list of symbol-value pairs, and at least one "
            "value to evaluate." );
    }

    CombinationValue::const_iterator it = combo->begin();
    assert( it != combo->end() ); // First of at least 3 - "let" (ignore)
    ++it;
    assert( it != combo->end() ); // Second of at least 3 - list of
                                  // symbol-values to define

    const CombinationValue* pairs = dynamic_cast<const CombinationValue*>(
        *it );
    if( !pairs )
    {
        throw EvaluationError( "The first operand to the let operator "
            "must be a combination containing symbol-value pairs.  '"
            + PrettyPrinter::Print( *it )
            + "' is not a combination." );
    }

    SymbolValuePairArray symvalpairs( pairs->size() );
    evaluate_symbol_values( pairs, ev, combo, symvalpairs );

    // Now that we have evaluated all the values we can insert them
    // into the environment.
    for( size_t idx = 0; idx != symvalpairs.size(); ++idx )
    {
        DefineUtilities::insert_value_into_environment( ev.GetEnvironment(),
            symvalpairs.GetSymbol( idx ), symvalpairs.GetValue( idx ) );
    }

    // Evaluate the body if the let except the last item (similar to
    // the end of Evaluation::EvalInContext).

    CombinationValue::const_iterator itletend = combo->end();
    assert( it != itletend ); // Since this let must have a body

    // Step back one - the last section of the body is handled differently
    --itletend;
    assert( it != itletend ); // Since this let must have a body

    ++it; // Skip past the symbol-value pairs to the body of the let

    // Evaluate all elements except the last one
    for( ; it != itletend; ++it )
    {
        // eval_in_context returns an auto_ptr, so
        // each returned value will be deleted.
        ev.SubEval( *it );
    }

    return *it;
}

/**
 * We have encountered a let that is not the end of the procedure -
 * we must wrap it in a lambda so the values are not remembered when
 * we exit the let.
 */
std::auto_ptr<Value> eval_let_not_tail_call( EvaluationContext& ev,
    const CombinationValue* combo )
{
    if( combo->size() < 3 )
    {
        throw EvaluationError(
            "Too few operands to the let operator: there should "
            "be at least 2: a list of symbol-value pairs, and at least one "
            "value to evaluate." );
    }

    CombinationValue::const_iterator it = combo->begin();
    assert( it != combo->end() ); // First of at least 3 - "let" (ignore)
    ++it;
    assert( it != combo->end() ); // Second of at least 3 - list of
                                  // symbol-values to define

    const CombinationValue* pairs = dynamic_cast<const CombinationValue*>(
        *it );
    if( !pairs )
    {
        throw EvaluationError( "The first operand to the let operator "
            "must be a combination containing symbol-value pairs.  '"
            + PrettyPrinter::Print( *it )
            + "' is not a combination." );
    }

    // Construct a new lambda function and the args to pass to it
    // TODO: lambdacall could be on the stack?
    std::auto_ptr<CombinationValue> lambdacall =
        std::auto_ptr<CombinationValue>( new CombinationValue );

    lambdacall->push_back( NULL ); // Create a slot where the (lambda ...)
                                   // will go.

    std::auto_ptr<CombinationValue> argnames = std::auto_ptr<CombinationValue>(
        new CombinationValue );

    for( CombinationValue::const_iterator itp = pairs->begin();
        itp != pairs->end(); ++itp )
    {
        const CombinationValue* pair = dynamic_cast<const CombinationValue*>(
            *itp );
        if( !pair )
        {
            throw EvaluationError( "Each item in the first argument to "
                "the let operator must be a combination containing a "
                "symbol and a value.  '"
                + PrettyPrinter::Print( *itp )
                + "' is not a combination." );
        }

        if( pair->size() != 2 )
        {
            throw EvaluationError( "Each item in the first argument to "
                "the let operator must be a combination containing a "
                "symbol and a value.  '"
                + PrettyPrinter::Print( *itp )
                + "' does not consist of 2 parts." );
        }

        CombinationValue::const_iterator itinpair = pair->begin();
        assert( itinpair != pair->end() ); // First of 2
        argnames->push_back( (*itinpair)->Clone() );
        ++itinpair;
        assert( itinpair != pair->end() ); // Second of 2
        lambdacall->push_back( (*itinpair)->Clone() );
    }

    std::auto_ptr<CombinationValue> lambdadefn =
        std::auto_ptr<CombinationValue>( new CombinationValue );

        lambdadefn->push_back( new LambdaSymbolValue );
    lambdadefn->push_back( argnames.release() );

    ++it;
    for( ; it != combo->end(); ++it )
    {
        lambdadefn->push_back( (*it)->Clone() );
    }

    std::auto_ptr<Value> lambda = LambdaUtilities::eval_lambda(
        lambdadefn.get(), ev.GetEnvironment() );

    *(lambdacall->begin()) = lambda.release();

    return ev.SubEval( lambdacall.get() );
}

}

//virtual
const std::string& LetSymbolValue::GetStringValue() const
{
    return StaticValue();
}

//static
const std::string& LetSymbolValue::StaticValue()
{
    static const std::string ret( "let" );
    return ret;
}

//virtual
LetSymbolValue* LetSymbolValue::Clone() const
{
    return new LetSymbolValue( *this );
}

//virtual
SpecialSymbolEvaluator::ESymbolType LetSymbolValue::Apply(
    EvaluationContext& ev, const CombinationValue* combo,
    std::auto_ptr<Value>& new_value, const Value*& existing_value ) const
{
    if( ev.GetIsTailCall() )
    {
        existing_value = process_let_tail_call( ev, combo );

        return SpecialSymbolEvaluator::eEvaluateExistingSymbol;
    }
    else
    {
        new_value = eval_let_not_tail_call( ev, combo );

        return SpecialSymbolEvaluator::eReturnNewValue;
    }
}
