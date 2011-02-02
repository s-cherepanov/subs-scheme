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

#include "lib/evaluationerror.h"
#include "lib/subsinterpreter.h"
#include "test/assertmacros.h"
#include "test/testdefinesymbol.h"

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



void define_is_scoped_within_its_parent_combination()
{
    SubsInterpreter interpreter;

    interpreter.Interpret(
        "(define foo 2)" );

    interpreter.Interpret(
        "(define (fn)"
        "        (define foo 3)"
        "        foo)" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "foo" ), "2" );
    TEST_ASSERT_EQUAL( interpreter.Interpret( "(fn)" ), "3" );
}



void define_overrides_special_symbols()
{
    SubsInterpreter interpreter;

    // Redefine what "define" means :)
    interpreter.Interpret( "(define define 2)" );

    TEST_ASSERT_EQUAL( interpreter.Interpret( "define" ), "2" );

    TEST_ASSERT_THROWS_BEGIN
    {
        // Now you can't define anything else :(
        interpreter.Interpret( "(define (fn) 3)" );
    }
    TEST_ASSERT_THROWS_END( "Undefined symbol 'fn'" )
}






}

#define SUITENAME "TestDefineSymbol"

void TestDefineSymbol::Run() const
{
    RUN_TEST(define_echoes_symbol_name);
    RUN_TEST(defined_symbol_yields_correct_value);
    RUN_TEST(defined_symbol_works_in_further_expression);
    RUN_TEST(too_many_arguments_to_define_is_an_error);
    RUN_TEST(too_few_arguments_to_define_is_an_error);
    RUN_TEST(define_in_terms_of_other_defined_symbol_succeds);
    RUN_TEST(define_in_terms_of_other_does_not_follow_other_when_changed);
    RUN_TEST(define_symbol_to_be_procedure_allows_evaluating_procedure);
    RUN_TEST(define_is_scoped_within_its_parent_combination);
    RUN_TEST(define_overrides_special_symbols);
}

