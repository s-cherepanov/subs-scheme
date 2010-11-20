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
#include <boost/shared_ptr.hpp>

#include "lib/value/symbol/specialsymbolvalue.h"
#include "lib/value/symbol/symbolvalue.h"
#include "lib/environment.h"
#include "lib/specialsymbolevaluator.h"

class Evaluator;

using namespace std;

SpecialSymbolEvaluator::SpecialSymbolEvaluator( Evaluator* evaluator,
    std::ostream& outstream )
: evaluator_( evaluator )
, outstream_( outstream )
//new_value_
, existing_value_( NULL )
{
}

bool SpecialSymbolEvaluator::IsSpecialSymbol( const SymbolValue& sym ) const
{
    return dynamic_cast<const SpecialSymbolValue*>( &sym );
}

SpecialSymbolEvaluator::ESymbolType
SpecialSymbolEvaluator::ProcessSpecialSymbol(
    const Value* optr, const CombinationValue* combo,
    boost::shared_ptr<Environment>& environment,
    bool is_tail_call )
{
    const SymbolValue* sym = dynamic_cast<const SymbolValue*>( optr );

    // If it's not a symbol, get out of here...
    if( !sym )
    {
        return eNoSpecialSymbol;
    }

    // If it's defined in the current environment, this overrides any
    // special symbols, so we bail out.
    if( environment->FindSymbol( sym->GetStringValue() ) )
    {
        return eNoSpecialSymbol;
    }

    // If it's a special symbol, Apply it.
    const SpecialSymbolValue* specsym =
        dynamic_cast<const SpecialSymbolValue*>( sym );

    if( specsym )
    {
        return specsym->Apply( evaluator_, combo, environment, new_value_,
            existing_value_, outstream_, is_tail_call );
    }

    // Otherwise it's just a normal symbol - continue.
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

