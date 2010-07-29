
#include <sstream>
#include <string>
#include <typeinfo>

#include "combinationvalue.h"
#include "integervalue.h"
#include "procedurevalue.h"
#include "symbolvalue.h"
#include "value.h"

#include "prettyprinter.h"

using namespace std;

namespace
{

void print_integer( const IntegerValue* value, ostream& result )
{
    result << value->GetIntValue();
}

void print_combination( const CombinationValue* value, ostream& result )
{
    result << "(";
    CombinationValue::const_iterator it = value->begin();
    while( true )
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


void print_procedure( const ProcedureValue* value, ostream& result )
{
    result << "<<PROCEDURE " << value->GetName() << ">>";
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

    const ProcedureValue* procedurevalue = dynamic_cast<
        const ProcedureValue*>( value );
    if( procedurevalue )
    {
        print_procedure( procedurevalue, result );
        return;
    }

    result << "<<UNPRINTABLE TYPE " << typeid(value).name() << ">>";
}

}

