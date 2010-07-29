
#include "assertmacros.h"
#include "lib/subsinterpreter.h"

#include "testlambda.h"

namespace
{


void define_lambda_noargs()
{
    SubsInterpreter interpreter;

    TEST_ASSERT_EQUAL( interpreter.Interpret(
        "(define foo (lambda () 3))" ),
        "foo" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "foo" ),
        "<<PROCEDURE user-defined>>" );
}




void define_lambda_args()
{
    SubsInterpreter interpreter;

    TEST_ASSERT_EQUAL( interpreter.Interpret(
        "(define foo (lambda (x) x))" ),
        "foo" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "foo" ),
        "<<PROCEDURE user-defined>>" );
}




void noargs_function_works()
{
    SubsInterpreter interpreter;

    TEST_ASSERT_EQUAL( interpreter.Interpret(
        "(define foo (lambda () (+ 1 2)))" ),
        "foo" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(foo)" ),
        "3" );
}




void noargs_function_works_list_body()
{
    SubsInterpreter interpreter;

    // The return value is 7 because the 5 and 6 are eval'd then ignored.
    TEST_ASSERT_EQUAL( interpreter.Interpret(
        "(define foo (lambda () 5 6 7))" ),
        "foo" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(foo)" ),
        "7" );
}





void double_function_works()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define double (lambda (x) (+ x x)))" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(double 3)" ), "6" );
}



}

void TestLambda::Run() const
{
    define_lambda_noargs();
    define_lambda_args();
    noargs_function_works();
    noargs_function_works_list_body();
    double_function_works();
    // NOTDONE arguments_dont_leak_out();
}

