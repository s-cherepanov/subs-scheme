
#include "environment.h"
#include "equalsprocedurevalue.h"
#include "falsevalue.h"
#include "truevalue.h"
#include "plusprocedurevalue.h"

#include "builtins.h"

namespace BuiltIns
{

void Init( Environment& environment )
{
    // TODO: don't allocate these on the stack?
    // TODO: make some or all of these immutable and thus not copied or deleted?
    environment.InsertSymbol( "#t", new TrueValue() );
    environment.InsertSymbol( "#f", new FalseValue() );
    environment.InsertSymbol( "+", new PlusProcedureValue() );
    environment.InsertSymbol( "=", new EqualsProcedureValue() );
}

}


