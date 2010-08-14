
#include <memory>
#include <string>

#include "assertmacros.h"
#include "evaldepthtracer.h"
#include "lib/subsinterpreter.h"
#include "lib/compoundprocedurevalue.h"

#include "testtailcalloptimisation.h"

using namespace std;

namespace
{


void count_optimised()
{
    SubsInterpreter interpreter;

    EvalDepthTracer tracer;
    interpreter.SetTracer( &tracer );

    interpreter.Interpret(
        "(define (count n)"
        "        (if (= n 1)"
        "            1"
        "            (count (- n 1))))"
        );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(count 1)" ), "1" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(count 2)" ), "1" );

    interpreter.Interpret( "(count 10)" );

    // Even though Lisp recursed to 10 levels, the C++ stack never got beyond 3
    TEST_ASSERT_EQUAL( tracer.GetMaxEvalDepth(), 3 );
}


void bad_count_cant_be_optimised()
{
    SubsInterpreter interpreter;

    EvalDepthTracer tracer;
    interpreter.SetTracer( &tracer );

    interpreter.Interpret(
        "(define (badcount n)"
        "        (if (= n 1)"
        "            1"
        "            (badcount (- n 1)))"
        "        1)" // pointless statement - e.g. could be a print
        );

    interpreter.Interpret( "(badcount 10)" );

    // Because the tail-call optimisation is not available, the C++ stack
    // had to go to as many levels as the Lisp stack
    TEST_ASSERT_EQUAL( tracer.GetMaxEvalDepth(), 13 );
}




}

void TestTailCallOptimisation::Run() const
{
    count_optimised();
    bad_count_cant_be_optimised();
}


