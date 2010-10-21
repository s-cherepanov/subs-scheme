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

#include <memory>
#include <sstream>

#include "lib/newlineprocessor.h"
#include "test/assertmacros.h"
#include "test/testnewlineprocessor.h"

using namespace std;

namespace
{


void no_indent_writes_dot_space()
{
    ostringstream ss;

    NewLineProcessor newline_processor( ss );

    newline_processor.NewLine();

    TEST_ASSERT_EQUAL( ss.str(), ". " );
}


void indent_writes_dot_space_indent()
{
    ostringstream ss;

    NewLineProcessor newline_processor( ss );

    newline_processor.PushIndent( 1, 5 );
    newline_processor.NewLine();

    TEST_ASSERT_EQUAL( ss.str(), ".      " );
}



void multiple_calls_write_multiple_times()
{
    ostringstream ss;

    NewLineProcessor newline_processor( ss );

    newline_processor.PushIndent( 1, 5 );
    newline_processor.NewLine();

    newline_processor.PushIndent( 2, 3 );
    newline_processor.NewLine();

    TEST_ASSERT_EQUAL( ss.str(),
        ".      "
        ".    " );
}




void stack_can_unwind()
{
    ostringstream ss;

    NewLineProcessor newline_processor( ss );

    newline_processor.PushIndent( 1, 5 );
    newline_processor.PushIndent( 2, 7 );
    newline_processor.PopIndent( 2 );
    newline_processor.NewLine();
    newline_processor.PopIndent( 1 );
    newline_processor.PopIndent( 2 ); // This does nothing...
    newline_processor.NewLine();

    TEST_ASSERT_EQUAL( ss.str(),
        ".      "
        ". " );
}





}

#define SUITENAME "TestNewLineProcessor"

void TestNewLineProcessor::Run() const
{
    RUN_TEST(no_indent_writes_dot_space);
    RUN_TEST(indent_writes_dot_space_indent);
    RUN_TEST(multiple_calls_write_multiple_times);
    RUN_TEST(stack_can_unwind);
}

