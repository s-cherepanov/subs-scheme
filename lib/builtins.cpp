
#include "environment.h"
#include "equalsnativefunctionvalue.h"
#include "falsevalue.h"
#include "minusnativefunctionvalue.h"
#include "plusnativefunctionvalue.h"
#include "timesnativefunctionvalue.h"
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
    InsertProcedure<PlusNativeFunctionValue>( environment );
    InsertProcedure<MinusNativeFunctionValue>( environment );
    InsertProcedure<TimesNativeFunctionValue>( environment );


    // Comparators
    InsertProcedure<EqualsNativeFunctionValue>( environment );
}

}


