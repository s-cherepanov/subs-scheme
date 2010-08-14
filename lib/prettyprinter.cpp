
#include <sstream>
#include <string>
#include <typeinfo>

#include "combinationvalue.h"
#include "falsevalue.h"
#include "integervalue.h"
#include "nativefunctionvalue.h"
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

