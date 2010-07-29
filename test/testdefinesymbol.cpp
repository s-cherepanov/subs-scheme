
#include "assertmacros.h"
#include "lib/evaluationerror.h"
#include "lib/subsinterpreter.h"

#include "testdefinesymbol.h"

namespace
{

void define_echoes_symbol_name()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(define foo 2)" ), "foo" );
}


void defined_symbol_yields_correct_value()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define foo 2)" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "foo" ), "2" );
}




void defined_symbol_works_in_further_expression()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define foo 2)" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(+ foo 1)" ), "3" );
}



void too_many_arguments_to_define_is_an_error()
{
    SubsInterpreter interpreter;

    bool exception_thrown = false;
    try
    {
        interpreter.Interpret( "(define foo 2 5)" );
    }
    catch( EvaluationError& e )
    {
        exception_thrown = true;
    }

    TEST_ASSERT_EQUAL( exception_thrown, true );
}



void too_few_arguments_to_define_is_an_error()
{
    SubsInterpreter interpreter;

    bool exception_thrown = false;
    try
    {
        interpreter.Interpret( "(define foo)" );
    }
    catch( EvaluationError& e )
    {
        exception_thrown = true;
    }

    TEST_ASSERT_EQUAL( exception_thrown, true );
}



void define_in_terms_of_other_defined_symbol_succeds()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define foo 2)" );
    interpreter.Interpret( "(define bar foo)" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "bar" ), "2" );
}



void define_in_terms_of_other_does_not_follow_other_when_changed()
{
    // This behaviour follows what plt-scheme does.

    SubsInterpreter interpreter;

    interpreter.Interpret( "(define foo 2)" );
    interpreter.Interpret( "(define bar foo)" );
    interpreter.Interpret( "(define foo 3)" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "bar" ), "2" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "foo" ), "3" );
}


void define_symbol_to_be_procedure_allows_evaluating_procedure()
{
    SubsInterpreter interpreter;

    interpreter.Interpret( "(define foobar +)" );
    interpreter.Interpret( "(define foo 4)" );
    interpreter.Interpret( "(define bar 3)" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "(foobar foo bar)" ), "7" );
}






}

void TestDefineSymbol::Run() const
{
    define_echoes_symbol_name();
    defined_symbol_yields_correct_value();
    defined_symbol_works_in_further_expression();
    too_many_arguments_to_define_is_an_error();
    too_few_arguments_to_define_is_an_error();
    define_in_terms_of_other_defined_symbol_succeds();
    define_in_terms_of_other_does_not_follow_other_when_changed();
    define_symbol_to_be_procedure_allows_evaluating_procedure();

    //NOTDONE define_is_scoped_within_its_parent_combination()
}

