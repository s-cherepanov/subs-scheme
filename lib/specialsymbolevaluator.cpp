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
#include <sstream>

#include "combinationvalue.h"
#include "compoundprocedurevalue.h"
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
    const Value* definition )
{
    environment.InsertSymbol( to_define->GetStringValue(),
        ev->EvalInContext( definition, environment ).release() );

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
    Environment& environment )
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
                combo->end(), sym->GetStringValue() ).get() );
    }
    else
    {
        if( combo->size() > 3 )
        {
            throw EvaluationError(
                "Too many operands to the define operator: there should "
                "be 2." );
        }

        const SymbolValue* sym = dynamic_cast<const SymbolValue*>( to_define );
        if( !sym )
        {
            throw EvaluationError( "The first operand to the define operator "
                "must be a symbol or a combination. '" + PrettyPrinter::Print(
                    to_define ) + "' is neither." );
        }

        return eval_define_symbol( ev, environment, sym, *it );
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


bool is_define_symbol( const SymbolValue& sym )
{
    // TODO: case insensitive?
    return ( sym.GetStringValue() == "define" );
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


std::auto_ptr<Value> eval_or( Evaluator* ev, const CombinationValue* combo,
    Environment& environment )
{
    CombinationValue::const_iterator it = combo->begin();
    assert( it != combo->end() );
    ++it;

    for( ; it != combo->end(); ++it )
    {
        std::auto_ptr<Value> value = ev->EvalInContext( *it, environment );
        if( ValueUtilities::IsTrue( value.get() ) )
        {
            return value;
        }
    }

    return std::auto_ptr<Value>( new FalseValue );
}


/**
 * Evaluate 'and'.  If all elements before the last one are true, returns
 * the last one.  Otherwise, return NULL, and set bool_ret to true if there
 * were no arguments, and false if one of the arguments was false.
 *
 */
const Value* eval_and( Evaluator* ev, const CombinationValue* combo,
    Environment& environment, bool& bool_ret )
{
    CombinationValue::const_iterator itlast = combo->end();
    assert( itlast != combo->begin() );
    --itlast;

    if( itlast == combo->begin() )
    {
        // There were no arguments - we must return true
        bool_ret = true;
        return NULL;
    }
    
    CombinationValue::const_iterator it = combo->begin();
    assert( it != combo->end() );
    ++it;

    for( ; it != itlast; ++it )
    {
        std::auto_ptr<Value> value = ev->EvalInContext( *it, environment );
        if( ValueUtilities::IsFalse( value.get() ) )
        {
            // One of the arguments was false - we must return false
            bool_ret = false;
            return NULL;
        }
    }

    // All arguments except the last were true - return the last (it may
    // be either true or false).
    return *itlast;
}


const Value* process_if( Evaluator* ev, const CombinationValue* combo,
    Environment& environment )
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
        environment );

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
    Environment& environment )
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
                environment );

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

SpecialSymbolEvaluator::SpecialSymbolEvaluator( Evaluator* evaluator )
: evaluator_( evaluator )
//new_value_
, existing_value_( NULL )
{
}

SpecialSymbolEvaluator::ESymbolType
SpecialSymbolEvaluator::ProcessSpecialSymbol(
    const CombinationValue* combo, Environment& environment )
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
        new_value_ = eval_define( evaluator_, combo, environment );

        return eReturnNewValue;
    }

    if( is_lambda_symbol( symref ) )
    {
        new_value_ = eval_lambda( combo );

        return eReturnNewValue;
    }

    if( is_if_symbol( symref ) )
    {
        existing_value_ = process_if( evaluator_, combo, environment );

        return eEvaluateExistingSymbol;
    }

    if( is_cond_symbol( symref ) )
    {
        existing_value_ = process_cond( evaluator_, combo, environment );

        return eEvaluateExistingSymbol;
    }

    if( is_or_symbol( symref ) )
    {
        new_value_ = eval_or( evaluator_, combo, environment );

        return eReturnNewValue;
    }

    if( is_and_symbol( symref ) )
    {
        bool bool_ret = false;
        existing_value_ = eval_and( evaluator_, combo, environment, bool_ret );

        if( existing_value_ )
        {
            return eEvaluateExistingSymbol;
        }
        else
        {
            if( bool_ret )
            {
                new_value_.reset( new TrueValue );
            }
            else
            {
                new_value_.reset( new FalseValue );
            }
            return eReturnNewValue;
        }
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

