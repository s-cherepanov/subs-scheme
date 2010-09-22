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

#include <cassert>
#include <memory>
#include <sstream>

#include "argschecker.h"
#include "combinationvalue.h"
#include "compoundprocedurevalue.h"
#include "displayevaluator.h"
#include "environment.h"
#include "evaluationerror.h"
#include "evaluator.h"
#include "falsevalue.h"
#include "prettyprinter.h"
#include "symbolvalue.h"
#include "specialsymbolevaluator.h"
#include "truevalue.h"
#include "valueutilities.h"

using namespace std;

namespace
{


std::auto_ptr<Value> eval_define_symbol( Evaluator* ev,
    Environment& environment, const SymbolValue* to_define,
    const Value* definition, std::ostream& outstream )
{
    environment.InsertSymbol( to_define->GetStringValue(),
        ev->EvalInContext( definition, environment, outstream, false
            ).release() );

    return auto_ptr<Value>( to_define->Clone() );
}


/**
 * Starting at it, clone Values into a new CombinationValue until we hit end.
 *
 * If the value of it supplied is the same as "begin()", this is identical
 * to calling "Clone()" on the combination we are iterating through.
 */
auto_ptr<CombinationValue> clone_partial_combo(
    CombinationValue::const_iterator it,
    const CombinationValue::const_iterator& end )
{
    auto_ptr<CombinationValue> ret( new CombinationValue );
    for( ; it != end; ++it )
    {
        ret->push_back( (*it)->Clone() );
    }
    return ret;
}


std::auto_ptr<Value> define_procedure(
    CombinationValue::const_iterator itarg,
    const CombinationValue::const_iterator& argsend,
    CombinationValue::const_iterator itbody,
    const CombinationValue::const_iterator& bodyend,
    const std::string& name = "" )
{
    return auto_ptr<Value>( new CompoundProcedureValue(
        clone_partial_combo( itarg, argsend ).release(),
        clone_partial_combo( itbody, bodyend ).release(), name ) );
}


std::auto_ptr<Value> eval_define( Evaluator* ev, const CombinationValue* combo,
    Environment& environment, std::ostream& outstream )
{
    if( combo->size() < 3 )
    {
        throw EvaluationError(
            "Too few operands to the define operator: there should be 2." );
    }

    CombinationValue::const_iterator it = combo->begin();
    assert( it != combo->end() );
    ++it;
    assert( it != combo->end() );

    const Value* to_define = *it;

    ++it;
    assert( it != combo->end() );

    const CombinationValue* comb_to_define =
        dynamic_cast<const CombinationValue*>( to_define );

    if( comb_to_define )
    {
        CombinationValue::const_iterator itarg = comb_to_define->begin();

        if( itarg == comb_to_define->end() )
        {
            throw EvaluationError(
                "The first operand to define must be a symbol or a non-empty "
                "combination.  The combination supplied is empty." );
        }

        const SymbolValue* sym = dynamic_cast<const SymbolValue*>( *itarg );
        if( !sym )
        {
            throw EvaluationError( "The first element in the list defining "
                "a procedure must be a symbol - '" + PrettyPrinter::Print(
                    to_define ) + "' is not a symbol." );
        }

        ++itarg; // Move on from the procedure name to the arguments

        return eval_define_symbol( ev, environment, sym,
            define_procedure( itarg, comb_to_define->end(), it,
                combo->end(), sym->GetStringValue() ).get(), outstream );
    }
    else
    {
        if( combo->size() > 3 )
        {
            ArgsChecker::ThrowWrongNumArgsException( "define",
                combo->size() - 1, 2 );
        }

        const SymbolValue* sym = dynamic_cast<const SymbolValue*>( to_define );
        if( !sym )
        {
            throw EvaluationError( "The first operand to the define operator "
                "must be a symbol or a combination. '" + PrettyPrinter::Print(
                    to_define ) + "' is neither." );
        }

        return eval_define_symbol( ev, environment, sym, *it, outstream );
    }
}

std::auto_ptr<Value> eval_lambda( const CombinationValue* combo )
{
    if( combo->size() < 3 )
    {
        throw EvaluationError(
            "Too few operands to the lambda operator: there should "
            "be  at least 2." );
    }

    CombinationValue::const_iterator it = combo->begin();
    assert( it != combo->end() ); // First of at least 3 - "lambda" (ignore)
    ++it;
    assert( it != combo->end() ); // Second of at least 3 - arguments

    const CombinationValue* args = dynamic_cast<const CombinationValue*>( *it );
    if( !args )
    {
        throw EvaluationError( "The first operand to the lambda operator "
            "must be a combination naming its arguments.  '"
            + PrettyPrinter::Print( *it )
            + "' is not a combination." );
    }

    ++it;
    assert( it != combo->end() ); // Third of at least 3 - procedure body

    // Copy the whole arguments combo as the args of the procedure, and
    // copy the rest of the main combo (unevaluated) as the body
    // of the function
    return define_procedure( args->begin(), args->end(), it, combo->end() );
}


/**
 * We have encountered a let whose return value is the return value of
 * the function we are in.  We can optimise by modelling this as defines
 * of the let variables, since this environment is disposable anyway,
 * so the leaking out of the defined values has no effect.
 */
const Value* process_let_tail_call( Evaluator* ev,
    const CombinationValue* combo, Environment& environment,
    std::ostream& outstream )
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

        const string& GetSymbol( size_t index )
        {
            assert( index < size_ );
            return symbols_array_[index];
        }

        auto_ptr<Value> GetValue( size_t index )
        {
            assert( index < size_ );
            return values_array_[index];
        }

        size_t size()
        {
            return size_;
        }

    private:
        size_t size_;
        string* symbols_array_;
        auto_ptr<Value>* values_array_;
    };

    SymbolValuePairArray symvalpairs( pairs->size() );

    // Go through all the symbol-value pairs, evaluating the
    // values remembering them along with the symbol names.
    // Don't insert them into the environment yet since we
    // don't want the definitions to interfere with each other.
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

        symvalpairs.Insert( count, name->GetStringValue(),
            ev->EvalInContext( value, environment, outstream, false ) );
    }

    // Now that we have evaluated all the values we can insert them
    // into the environment.
    for( size_t idx = 0; idx != symvalpairs.size(); ++idx )
    {
        environment.InsertSymbol( symvalpairs.GetSymbol( idx ),
            symvalpairs.GetValue( idx ).release() );
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
        ev->EvalInContext( *it, environment, outstream, false );
    }

    return *it;
}

/**
 * We have encountered a let that is not the end of the procedure -
 * we must wrap it in a lambda so the values are not remembered when
 * we exit the let.
 */
std::auto_ptr<Value> eval_let_not_tail_call( Evaluator* ev,
    const CombinationValue* combo, Environment& environment,
    std::ostream& outstream )
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

    lambdadefn->push_back( new SymbolValue( "lambda" ) );
    lambdadefn->push_back( argnames.release() );

    ++it;
    for( ; it != combo->end(); ++it )
    {
        lambdadefn->push_back( (*it)->Clone() );
    }

    std::auto_ptr<Value> lambda = eval_lambda( lambdadefn.get() );

    *(lambdacall->begin()) = lambda.release();

    return ev->EvalInContext( lambdacall.get(), environment, outstream, false );
}


bool is_define_symbol( const SymbolValue& sym )
{
    // TODO: case insensitive?
    return ( sym.GetStringValue() == "define" );
}

bool is_let_symbol( const SymbolValue& sym )
{
    // TODO: case insensitive?
    return ( sym.GetStringValue() == "let" );
}

bool is_if_symbol( const SymbolValue& sym )
{
    // TODO: case insensitive?
    return ( sym.GetStringValue() == "if" );
}

bool is_cond_symbol( const SymbolValue& sym )
{
    // TODO: case insensitive?
    return ( sym.GetStringValue() == "cond" );
}


bool is_lambda_symbol( const SymbolValue& sym )
{
    // TODO: case insensitive?
    return ( sym.GetStringValue() == "lambda" );
}


bool is_or_symbol( const SymbolValue& sym )
{
    // TODO: case insensitive?
    return ( sym.GetStringValue() == "or" );
}


bool is_and_symbol( const SymbolValue& sym )
{
    // TODO: case insensitive?
    return ( sym.GetStringValue() == "and" );
}


class AndProperties
{
public:
    static bool EarlyExit( const Value* value )
    {
        return ValueUtilities::IsFalse( value );
    }

    static std::auto_ptr<Value> NoArgumentsReturnValue()
    {
        return std::auto_ptr<Value>( new TrueValue );
    }
};


class OrProperties
{
public:
    static bool EarlyExit( const Value* value )
    {
        return ValueUtilities::IsTrue( value );
    }

    static std::auto_ptr<Value> NoArgumentsReturnValue()
    {
        return std::auto_ptr<Value>( new FalseValue );
    }
};


/**
 * Evaluate 'and' or 'or'.  If we can tail-call optimise, we return a
 * pointer to the existing value that can be evaluated.  Otherwise,
 * set new_value_ to the newly-allocated value to return.
 */
template<class PredicateProperties>
const Value* eval_predicate( Evaluator* ev, const CombinationValue* combo,
    Environment& environment, std::auto_ptr<Value>& new_value_,
    std::ostream& outstream, bool is_tail_call )
{
    CombinationValue::const_iterator itlast = combo->end();
    assert( itlast != combo->begin() );
    --itlast;

    if( itlast == combo->begin() )
    {
        // There were no arguments - we must return the correct answer
        new_value_ = PredicateProperties::NoArgumentsReturnValue();
        return NULL;
    }
    
    CombinationValue::const_iterator it = combo->begin();
    assert( it != combo->end() );
    ++it;

    for( ; it != itlast; ++it )
    {
        std::auto_ptr<Value> value = ev->EvalInContext( *it, environment,
            outstream, is_tail_call );
        if( PredicateProperties::EarlyExit( value.get() ) )
        {
            // One of the arguments allow us to exit early - set the answer
            new_value_ = value;
            return NULL;
        }
    }

    // None of the early arguments caused us to exit - return the last (it may
    // be either true or false).
    return *itlast;
}


const Value* process_if( Evaluator* ev, const CombinationValue* combo,
    Environment& environment, std::ostream& outstream, bool is_tail_call )
{
    // TODO: only one if needed here (in the normal case)

    if( combo->size() < 4 )
    {
        throw EvaluationError(
            "Not enough operands to if: there should be 3 (predicate, "
            "true-value, else-value)." );
    }

    if( combo->size() > 4 )
    {
        throw EvaluationError(
            "Too many operands to if: there should be 3 (predicate, "
            "true-value, else-value)." );
    }

    CombinationValue::const_iterator it = combo->begin();
    assert( it != combo->end() ); // "if" - there are 4 items

    ++it;
    assert( it != combo->end() ); // predicate - there are 4 items
    const Value* predicate = *it;

    ++it; // Move to "true" value
    assert( it != combo->end() );

    std::auto_ptr<Value> evald_pred = ev->EvalInContext( predicate,
        environment, outstream, is_tail_call );

    if( ValueUtilities::IsFalse( evald_pred.get() ) )
    {
        ++it; // Move to "false" value
        assert( it != combo->end() );
    }

    return *it;
}



const Value* get_test_from_testvalue_pair( const Value* pair )
{
    const CombinationValue* paircombo = dynamic_cast<const CombinationValue*>(
        pair );

    if( !paircombo )
    {
        throw EvaluationError( "The operands to 'cond' must be pairs.  '"
            + PrettyPrinter::Print( pair )
            + "' is not a combination." );
    }

    if( paircombo->size() != 2 )
    {
        ostringstream ss;
        ss  << "The operands to 'cond' must be pairs.  '"
            << PrettyPrinter::Print( pair )
            << "' has " << paircombo->size()
            << " elements, where it should have 2.";

        throw EvaluationError( ss.str() );
    }

    return *paircombo->begin();
}

const Value* get_value_from_testvalue_pair( const Value* pair )
{
    // TODO: avoid doing the same cast here and in get_test_from_testvalue_pair
    const CombinationValue* paircombo = dynamic_cast<const CombinationValue*>(
        pair );

    assert( paircombo ); // We shouldn't get here without checking this in
                         // get_test_from_testvalue_pair
    assert( paircombo->size() == 2 ); // We shouldn't get here without
                                      // checking this in
                                      // get_test_from_testvalue_pair

    CombinationValue::const_iterator it = paircombo->begin();
    ++it;

    return *it;
}

bool is_else( const Value* value )
{
    // TODO: break this file into several (and test these functions?)

    const SymbolValue* sym = dynamic_cast<const SymbolValue*>( value );

    // TODO: case insensitive?
    return ( sym && sym->GetStringValue() == "else" );
}

const Value* process_cond( Evaluator* ev, const CombinationValue* combo,
    Environment& environment, std::ostream& outstream, bool is_tail_call )
{
    // Look for pairs of predicate and value
    // or "else" and value, which must be last.

    if( combo->size() < 2 )
    {
        throw EvaluationError(
            "Not enough operands to cond: there should be at least 1 "
            "test-value pair." );
    }

    CombinationValue::const_iterator it = combo->begin();
    assert( it != combo->end() ); // "cond" - there are at least 2 items

    ++it;
    assert( it != combo->end() ); // test-value pair - there must be at least 1

    for( ; it != combo->end(); ++it )
    {
        const Value* test_value_pair = *it;

        const Value* test = get_test_from_testvalue_pair( test_value_pair );

        if( is_else( test ) )
        {
            ++it;
            if( it != combo->end() )
            {
                throw EvaluationError( "The else pair must be the last pair "
                    "in a 'cond' expression.  This value was found after the "
                    "else: '" + PrettyPrinter::Print( *it ) + "'." );
            }

            return get_value_from_testvalue_pair( test_value_pair );
        }
        else
        {
            std::auto_ptr<Value> evald_test = ev->EvalInContext( test,
                environment, outstream, is_tail_call );

            if( ValueUtilities::IsTrue( evald_test.get() ) )
            {
                return get_value_from_testvalue_pair( test_value_pair );
            }
        }
    }

    // None of the conditions were true and there was no else - return NULL.
    return NULL;
}


}

SpecialSymbolEvaluator::SpecialSymbolEvaluator( Evaluator* evaluator,
    std::ostream& outstream )
: evaluator_( evaluator )
, outstream_( outstream )
//new_value_
, existing_value_( NULL )
{
}

SpecialSymbolEvaluator::ESymbolType
SpecialSymbolEvaluator::ProcessSpecialSymbol(
    const CombinationValue* combo, Environment& environment, bool is_tail_call )
{
    // Check to see whether it's a special symbol
    const SymbolValue* sym = dynamic_cast<const SymbolValue*>(
        *( combo->begin() ) );

    if( !sym )
    {
        return eNoSpecialSymbol;
    }

    const SymbolValue& symref = *sym;

    if( is_define_symbol( symref ) )
    {
        new_value_ = eval_define( evaluator_, combo, environment, outstream_ );

        return eReturnNewValue;
    }

    if( is_lambda_symbol( symref ) )
    {
        new_value_ = eval_lambda( combo );

        return eReturnNewValue;
    }

    if( is_let_symbol( symref ) )
    {
        if( is_tail_call )
        {
            existing_value_ = process_let_tail_call( evaluator_, combo,
                environment, outstream_ );
            return eEvaluateExistingSymbol;
        }
        else
        {
            new_value_ = eval_let_not_tail_call( evaluator_, combo,
                environment, outstream_ );
            return eReturnNewValue;
        }
    }

    if( is_if_symbol( symref ) )
    {
        existing_value_ = process_if( evaluator_, combo, environment,
            outstream_, is_tail_call );

        return eEvaluateExistingSymbol;
    }

    if( is_cond_symbol( symref ) )
    {
        existing_value_ = process_cond( evaluator_, combo, environment,
            outstream_, is_tail_call );

        return eEvaluateExistingSymbol;
    }

    if( is_or_symbol( symref ) )
    {
        existing_value_ = eval_predicate<OrProperties>( evaluator_, combo,
            environment, new_value_, outstream_, is_tail_call );

        if( existing_value_ )
        {
            assert( !new_value_.get() );
            return eEvaluateExistingSymbol;
        }
        else
        {
            assert( new_value_.get() );
            return eReturnNewValue;
        }
    }

    if( is_and_symbol( symref ) )
    {
        existing_value_ = eval_predicate<AndProperties>( evaluator_, combo,
            environment, new_value_, outstream_, is_tail_call );

        if( existing_value_ )
        {
            assert( !new_value_.get() );
            return eEvaluateExistingSymbol;
        }
        else
        {
            assert( new_value_.get() );
            return eReturnNewValue;
        }
    }

    if( DisplayEvaluator::ProcessDisplaySymbol( evaluator_, combo,
            environment, symref, outstream_ ) )
    {
        existing_value_ = NULL;
        return eEvaluateExistingSymbol;
    }

    return eNoSpecialSymbol;
}

std::auto_ptr<Value> SpecialSymbolEvaluator::NewValue()
{
    assert( new_value_.get() );

    return new_value_;
}

const Value* SpecialSymbolEvaluator::ExistingValue()
{
    assert( !new_value_.get() );

    return existing_value_;
}

