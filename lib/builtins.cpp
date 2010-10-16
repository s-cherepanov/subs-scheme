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

#include "cosnativefunctionvalue.h"
#include "environment.h"
#include "dividenativefunctionvalue.h"
#include "equalnativefunctionvalue.h"
#include "equalsnativefunctionvalue.h"
#include "evennativefunctionvalue.h"
#include "expnativefunctionvalue.h"
#include "falsevalue.h"
#include "greaterthannativefunctionvalue.h"
#include "lessthannativefunctionvalue.h"
#include "lognativefunctionvalue.h"
#include "minnativefunctionvalue.h"
#include "minusnativefunctionvalue.h"
#include "maxnativefunctionvalue.h"
#include "negativenativefunctionvalue.h"
#include "nilvalue.h"
#include "notnativefunctionvalue.h"
#include "nullnativefunctionvalue.h"
#include "oddnativefunctionvalue.h"
#include "pairnativefunctionvalue.h"
#include "plusnativefunctionvalue.h"
#include "positivenativefunctionvalue.h"
#include "remaindernativefunctionvalue.h"
#include "sinnativefunctionvalue.h"
#include "tannativefunctionvalue.h"
#include "timesnativefunctionvalue.h"
#include "truevalue.h"
#include "zeronativefunctionvalue.h"

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
    environment.InsertSymbol( "nil", new NilValue );

    // Arithmetic operations
    InsertProcedure<PlusNativeFunctionValue>(   environment );
    InsertProcedure<MinusNativeFunctionValue>(  environment );
    InsertProcedure<TimesNativeFunctionValue>(  environment );
    InsertProcedure<DivideNativeFunctionValue>( environment );

    // Comparators
    InsertProcedure<EqualsNativeFunctionValue>(      environment );
    InsertProcedure<GreaterThanNativeFunctionValue>( environment );
    InsertProcedure<LessThanNativeFunctionValue>(    environment );

    // Boolean logic
    InsertProcedure<NotNativeFunctionValue>( environment );
    // and and or are not procedures, so are dealt with
    // in SpecialSymbolEvaluator.

    // Mathematical functions
    InsertProcedure<CosNativeFunctionValue>(       environment );
    InsertProcedure<ExpNativeFunctionValue>(       environment );
    InsertProcedure<EvenNativeFunctionValue>(      environment );
    InsertProcedure<LogNativeFunctionValue>(       environment );
    InsertProcedure<MaxNativeFunctionValue>(       environment );
    InsertProcedure<MinNativeFunctionValue>(       environment );
    InsertProcedure<NegativeNativeFunctionValue>(  environment );
    InsertProcedure<OddNativeFunctionValue>(       environment );
    InsertProcedure<PositiveNativeFunctionValue>(  environment );
    InsertProcedure<RemainderNativeFunctionValue>( environment );
    InsertProcedure<SinNativeFunctionValue>(       environment );
    InsertProcedure<TanNativeFunctionValue>(       environment );
    InsertProcedure<ZeroNativeFunctionValue>(     environment );

    // Question functions
    InsertProcedure<EqualNativeFunctionValue>(    environment );
    InsertProcedure<PairNativeFunctionValue>(     environment );
    InsertProcedure<NullNativeFunctionValue>(     environment );
}

}


