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

#include <cstring>
#include <iosfwd>
#include <memory>
#include <string>

#include "lib/value/basic/combinationvalue.h"
#include "lib/value/basic/pairvalue.h"
#include "lib/value/symbol/cadrsymbolvalue.h"
#include "lib/value/value.h"
#include "lib/argschecker.h"
#include "lib/environment.h"
#include "lib/evaluationerror.h"
#include "lib/evaluator.h"
#include "lib/prettyprinter.h"
#include "lib/specialsymbolevaluator.h"

using namespace std;

namespace
{

SpecialSymbolEvaluator::ESymbolType apply_car_or_cdr(
    Evaluator* evaluator, const CombinationValue* combo,
    boost::shared_ptr<Environment>& environment,
    std::auto_ptr<Value>& new_value, const Value*& existing_value,
    std::ostream& outstream, bool is_tail_call, char a_or_d,
    const std::string& token_name )
{

    if( combo->size() != 2 )
    {
        ArgsChecker::ThrowWrongNumArgsException( token_name.c_str(),
            combo->size() - 1, 1 );
    }

    CombinationValue::const_iterator it = combo->begin();
    assert( it != combo->end() ); // "car" or "cdr" - there are 2 items

    ++it;
    assert( it != combo->end() ); // the pair to extract from

    std::auto_ptr<Value> evald_arg = evaluator->EvalInContext( *it,
        environment, outstream, false );

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

    new_value = evaluator->EvalInContext( selected, environment,
        outstream, false );

    return SpecialSymbolEvaluator::eReturnNewValue;
}

}

//private
CadrSymbolValue::CadrSymbolValue( const std::string& token_name )
: token_name_( token_name )
{
    memset( ad_list_, 0, sizeof( ad_list_ ) / sizeof( char ) );
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
    Evaluator* evaluator, const CombinationValue* combo,
    boost::shared_ptr<Environment>& environment,
    std::auto_ptr<Value>& new_value, const Value*& existing_value,
    std::ostream& outstream, bool is_tail_call ) const
{
    return apply_car_or_cdr( evaluator, combo, environment, new_value,
            existing_value, outstream, is_tail_call, ad_list_[0], token_name_ );
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
        if( counter > MAX_AD_LIST_SIZE )
        {
            return auto_ptr<Value>( NULL );
        }

        char ch = *it;
        if( ch == 'a' || ch == 'd' )
        {
            ret.ad_list_[counter] = ch;
        }
        else
        {
            // Contains a symbol not 'a' or 'd'
            return auto_ptr<Value>( NULL );
        }
    }

    return auto_ptr<Value>( ret.Clone() );
}

