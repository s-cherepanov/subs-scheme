
#include "assertmacros.h"
#include "lib/subsinterpreter.h"

#include "testif.h"

namespace
{

void true_and_false()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "#t" ), "<<True>>" );
}

void basic_if()
{
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(if #t 10 20)" ), "10" );
    TEST_ASSERT_EQUAL( SubsInterpreter().Interpret( "(if #f 10 20)" ), "20" );
}





}

void TestIf::Run() const
{
    //FAILS true_and_false();
    //FAILS basic_if();
    //FAILS everything_is_true_except_hash_f();
}

