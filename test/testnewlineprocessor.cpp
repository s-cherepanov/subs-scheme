
#include <memory>
#include <sstream>

#include "assertmacros.h"
#include "lib/newlineprocessor.h"

#include "testnewlineprocessor.h"

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

void TestNewLineProcessor::Run() const
{
    no_indent_writes_dot_space();
    indent_writes_dot_space_indent();
    multiple_calls_write_multiple_times();
    stack_can_unwind();
}

