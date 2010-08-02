
#include "environment.h"
#include "equalsprocedurevalue.h"
#include "falsevalue.h"
#include "minusprocedurevalue.h"
#include "plusprocedurevalue.h"
#include "timesprocedurevalue.h"
#include "truevalue.h"

#include "builtins.h"

namespace
{

template<class T>
void InsertProcedure( Environment& environment )
{
    environment.InsertSymbol( T::StaticName(), new T );
}

}


namespace BuiltIns
{

void Init( Environment& environment )
{
    // TODO: don't allocate these on the stack?
    // TODO: make some or all of these immutable and thus not copied or deleted?

    // Constants
    environment.InsertSymbol( "#t", new TrueValue );
    environment.InsertSymbol( "#f", new FalseValue );

    // Arithmetic operations
    InsertProcedure<PlusProcedureValue>( environment );
    InsertProcedure<MinusProcedureValue>( environment );
    InsertProcedure<TimesProcedureValue>( environment );


    // Comparators
    InsertProcedure<EqualsProcedureValue>( environment );
}

}

