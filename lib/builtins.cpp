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

#include "lib/value/basic/falsevalue.h"
#include "lib/value/basic/nilvalue.h"
#include "lib/value/basic/truevalue.h"
#include "lib/value/nativefunction/cosnativefunctionvalue.h"
#include "lib/value/nativefunction/dividenativefunctionvalue.h"
#include "lib/value/nativefunction/eqnativefunctionvalue.h"
#include "lib/value/nativefunction/equalnativefunctionvalue.h"
#include "lib/value/nativefunction/equalsnativefunctionvalue.h"
#include "lib/value/nativefunction/evennativefunctionvalue.h"
#include "lib/value/nativefunction/expnativefunctionvalue.h"
#include "lib/value/nativefunction/foreachnativefunctionvalue.h"
#include "lib/value/nativefunction/greaterthannativefunctionvalue.h"
#include "lib/value/nativefunction/lessthannativefunctionvalue.h"
#include "lib/value/nativefunction/loadnativefunctionvalue.h"
#include "lib/value/nativefunction/lognativefunctionvalue.h"
#include "lib/value/nativefunction/minnativefunctionvalue.h"
#include "lib/value/nativefunction/minusnativefunctionvalue.h"
#include "lib/value/nativefunction/maxnativefunctionvalue.h"
#include "lib/value/nativefunction/negativenativefunctionvalue.h"
#include "lib/value/nativefunction/notnativefunctionvalue.h"
#include "lib/value/nativefunction/nullnativefunctionvalue.h"
#include "lib/value/nativefunction/numbernativefunctionvalue.h"
#include "lib/value/nativefunction/oddnativefunctionvalue.h"
#include "lib/value/nativefunction/pairnativefunctionvalue.h"
#include "lib/value/nativefunction/plusnativefunctionvalue.h"
#include "lib/value/nativefunction/positivenativefunctionvalue.h"
#include "lib/value/nativefunction/remaindernativefunctionvalue.h"
#include "lib/value/nativefunction/sinnativefunctionvalue.h"
#include "lib/value/nativefunction/symbolnativefunctionvalue.h"
#include "lib/value/nativefunction/tannativefunctionvalue.h"
#include "lib/value/nativefunction/timesnativefunctionvalue.h"
#include "lib/value/nativefunction/zeronativefunctionvalue.h"
#include "lib/environment.h"
#include "lib/builtins.h"

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
    InsertProcedure<ZeroNativeFunctionValue>(      environment );

    // Questions
    InsertProcedure<EqNativeFunctionValue>(     environment );
    InsertProcedure<EqualNativeFunctionValue>(  environment );
    InsertProcedure<NullNativeFunctionValue>(   environment );
    InsertProcedure<NumberNativeFunctionValue>( environment );
    InsertProcedure<PairNativeFunctionValue>(   environment );
    InsertProcedure<SymbolNativeFunctionValue>( environment );

    // Scheme evaluation
    InsertProcedure<LoadNativeFunctionValue>( environment );

    // List operations
    InsertProcedure<ForEachNativeFunctionValue>( environment );
}

}


