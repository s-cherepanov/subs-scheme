/*
 *  Subs Scheme Interpreter
 *  Copyright (C) 2010, 2011 Andy Balaam
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

#include <cstring>
#include <iosfwd>
#include <memory>
#include <string>

#include "lib/value/basic/combinationvalue.h"
#include "lib/value/basic/pairvalue.h"
#include "lib/value/symbol/cadrsymbolvalue.h"
#include "lib/value/symbol/listsymbolvalue.h"
#include "lib/value/value.h"
#include "lib/argschecker.h"
#include "lib/environment.h"
#include "lib/evaluationcontext.h"
#include "lib/evaluationerror.h"
#include "lib/evaluator.h"
#include "lib/prettyprinter.h"
#include "lib/specialsymbolevaluator.h"

using namespace std;

namespace
{

void apply_car_or_cdr(
    EvaluationContext& ev, const Value* arg, char a_or_d,
    const std::string& token_name, std::auto_ptr<Value>& new_value )
{
    std::auto_ptr<Value> evald_arg = ev.SubEval( arg );

    PairValue* pair = dynamic_cast<PairValue*>( evald_arg.get() );

    if( !pair )
    {
        throw EvaluationError( "The argument to " + token_name
            + " must be a pair. '" +
            PrettyPrinter::Print( evald_arg.get() ) + "' is not a pair." );
    }

    const Value* selected;

    if( a_or_d == 'a' )
    {
        selected = pair->GetFirst();
    }
    else
    {
        selected = pair->GetSecond();
    }

    new_value.reset( selected->Clone() );
}

void apply_all( EvaluationContext& ev, const Value* arg,
    const string& token_name, std::auto_ptr<Value>& new_value )
{
    string::const_reverse_iterator it = token_name.rbegin();
    assert( it != token_name.rend() );
    ++it; // Skip the 'r' at the end
    assert( it != token_name.rend() );

    char ch = *it;
    assert( ch != 'c' ); // There must be at least one a or d
    const Value* intermediate_arg = arg;

    do
    {
        apply_car_or_cdr( ev, intermediate_arg, ch, token_name, new_value );

        intermediate_arg = new_value.get();
        ++it;
        assert( it != token_name.rend() ); // Since there will always be a c
                                           // at the beginning
        ch = *it;
    } while( ch != 'c' );
}


}

//private
CadrSymbolValue::CadrSymbolValue( const std::string& token_name )
: token_name_( token_name )
{
}

//virtual
const std::string& CadrSymbolValue::GetStringValue() const
{
    return token_name_;
}

//virtual
CadrSymbolValue* CadrSymbolValue::Clone() const
{
    return new CadrSymbolValue( *this );
}

//virtual
SpecialSymbolEvaluator::ESymbolType CadrSymbolValue::Apply(
    EvaluationContext& ev, const CombinationValue* combo,
    std::auto_ptr<Value>& new_value, const Value*& existing_value ) const
{
    if( combo->size() != 2 )
    {
        ArgsChecker::ThrowWrongNumArgsException( token_name_.c_str(),
            combo->size() - 1, 1 );
    }

    CombinationValue::const_iterator it = combo->begin();
    assert( it != combo->end() ); // "car" or "cdr" - there are 2 items

    ++it;
    assert( it != combo->end() ); // the pair to extract from

    apply_all( ev, *it, token_name_, new_value );

    return SpecialSymbolEvaluator::eReturnNewValue;
}


//static
std::auto_ptr<Value> CadrSymbolValue::CreateFromString(
    const std::string& token_name )
{
    assert( !token_name.empty() );

    string::const_iterator it = token_name.begin();

    // Doesn't start with c
    if( *it != 'c' )
    {
        return auto_ptr<Value>( NULL );
    }

    // Doesn't end with r
    if( *token_name.rbegin() != 'r' )
    {
        return auto_ptr<Value>( NULL );
    }

    string::const_iterator itend = token_name.end() - 1;

    ++it;
    assert( it != token_name.end() ); // Since we've seen c and r already,
                                      // the token must be >= 2 chars.

    // Just "cr"
    if( it == itend )
    {
        return auto_ptr<Value>( NULL );
    }

    CadrSymbolValue ret( token_name );

    unsigned int counter = 0;
    for( ; it != itend; ++it, ++counter )
    {
        // Too long
        if( counter >= MAX_AD_LIST_SIZE )
        {
            return auto_ptr<Value>( NULL );
        }

        char ch = *it;
        if( ch != 'a' && ch != 'd' )
        {
            // Contains a symbol not 'a' or 'd'
            return auto_ptr<Value>( NULL );
        }
    }

    return auto_ptr<Value>( ret.Clone() );
}

