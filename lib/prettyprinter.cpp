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

#include <sstream>
#include <string>
#include <typeinfo>

#include "combinationvalue.h"
#include "decimalvalue.h"
#include "falsevalue.h"
#include "integervalue.h"
#include "nativefunctionvalue.h"
#include "stringvalue.h"
#include "symbolvalue.h"
#include "truevalue.h"
#include "compoundprocedurevalue.h"
#include "value.h"

#include "prettyprinter.h"

using namespace std;

namespace
{

void print_integer( const IntegerValue* value, ostream& result )
{
    result << value->GetIntValue();
}


void print_decimal( const DecimalValue* value, ostream& result )
{
    double doublevalue = value->GetDoubleValue();

    ostringstream ss;
    ss << doublevalue;
    string str = ss.str();

    result << str;

    if( str.find( '.' ) == string::npos )
    {
        result << ".0";
    }
}


void print_string( const StringValue* value, ostream& result )
{
    result << "\"" << value->GetStringValue() << "\"";
}


void print_true( const TrueValue*, ostream& result )
{
    result << "#t";
}


void print_false( const FalseValue*, ostream& result )
{
    result << "#f";
}

void print_combination( const CombinationValue* value, ostream& result )
{
    result << "(";
    CombinationValue::const_iterator it = value->begin();
    while( it != value->end() )
    {
        PrettyPrinter::Print( *it, result );

        ++it;

        if( it == value->end() )
        {
            break;
        }
        else
        {
            result << " ";
        }
    }
    result << ")";
}


void print_symbol( const SymbolValue* value, ostream& result )
{
    result << value->GetStringValue();
}


void print_built_in_procedure( const NativeFunctionValue* value, ostream& result )
{
    result << "<<NATIVE-PROCEDURE " << value->GetName() << ">>";
}


void print_compound_procedure( const CompoundProcedureValue* value,
    ostream& result )
{
    result << "<<" << value->GetName() << ">>";
}



}

namespace PrettyPrinter
{

std::string Print( const Value* value )
{
    ostringstream result;
    Print( value, result );
    return result.str();
}

void Print( const Value* value, std::ostream& result )
{
    // TODO: switch on typeid?

    if( !value )
    {
        return;
    }

    const IntegerValue* integervalue = dynamic_cast<const IntegerValue*>(
        value );
    if( integervalue )
    {
        print_integer( integervalue, result );
        return;
    }

    const DecimalValue* decimalvalue = dynamic_cast<const DecimalValue*>(
        value );
    if( decimalvalue )
    {
        print_decimal( decimalvalue, result );
        return;
    }

    const StringValue* stringvalue = dynamic_cast<const StringValue*>(
        value );
    if( stringvalue )
    {
        print_string( stringvalue, result );
        return;
    }

    const TrueValue* truevalue = dynamic_cast<const TrueValue*>(
        value );
    if( truevalue )
    {
        print_true( truevalue, result );
        return;
    }

    const FalseValue* falsevalue = dynamic_cast<const FalseValue*>(
        value );
    if( falsevalue )
    {
        print_false( falsevalue, result );
        return;
    }

    const CombinationValue* combovalue = dynamic_cast<
        const CombinationValue*>( value );
    if( combovalue )
    {
        print_combination( combovalue, result );
        return;
    }

    const SymbolValue* symbolvalue = dynamic_cast<
        const SymbolValue*>( value );
    if( symbolvalue )
    {
        print_symbol( symbolvalue, result );
        return;
    }

    const NativeFunctionValue* fnvalue = dynamic_cast<
        const NativeFunctionValue*>( value );
    if( fnvalue )
    {
        print_built_in_procedure( fnvalue, result );
        return;
    }

    const CompoundProcedureValue* procvalue = dynamic_cast<
        const CompoundProcedureValue*>( value );
    if( procvalue )
    {
        print_compound_procedure( procvalue, result );
        return;
    }

    result << "<<UNPRINTABLE TYPE " << typeid(value).name() << ">>";
}

}

