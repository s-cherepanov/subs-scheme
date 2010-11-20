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
#include <sstream>
#include <string>

#include "lib/value/basic/combinationvalue.h"
#include "lib/value/symbol/condsymbolvalue.h"
#include "lib/value/symbol/elsesymbolvalue.h"
#include "lib/value/value.h"
#include "lib/environment.h"
#include "lib/evaluationerror.h"
#include "lib/evaluator.h"
#include "lib/prettyprinter.h"
#include "lib/specialsymbolevaluator.h"
#include "lib/valueutilities.h"

using namespace std;

namespace
{

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
    return ( dynamic_cast< const ElseSymbolValue* >( value ) );
}


const Value* process_cond( Evaluator* ev, const CombinationValue* combo,
    boost::shared_ptr<Environment>& environment, std::ostream& outstream )
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
                environment, outstream, false );

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


//virtual
const std::string& CondSymbolValue::GetStringValue() const
{
    return StaticValue();
}

//static
const std::string& CondSymbolValue::StaticValue()
{
    static const std::string ret( "cond" );
    return ret;
}

//virtual
CondSymbolValue* CondSymbolValue::Clone() const
{
    return new CondSymbolValue( *this );
}

//virtual
SpecialSymbolEvaluator::ESymbolType CondSymbolValue::Apply(
    Evaluator* evaluator, const CombinationValue* combo,
    boost::shared_ptr<Environment>& environment,
    std::auto_ptr<Value>& new_value, const Value*& existing_value,
    std::ostream& outstream, bool is_tail_call ) const
{
    existing_value = process_cond( evaluator, combo, environment, outstream );

    return SpecialSymbolEvaluator::eEvaluateExistingSymbol;
}

