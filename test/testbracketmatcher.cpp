
#include <sstream>

#include "assertmacros.h"
#include "lib/bracketmatcher.h"
#include "lib/lexer.h"
#include "lib/tokenlist.h"

#include "testbracketmatcher.h"

using namespace std;

namespace
{


void empty_input_yields_empty_list()
{
    istringstream ss( "" );
    ostringstream outss;
    Lexer lexer( ss );
    BracketMatcher matcher( lexer, outss, true );

    TEST_ASSERT_TRUE( matcher.ReadCombination().empty() );

    TEST_ASSERT_EQUAL( outss.str(), "" );
}


void single_token_yields_one_item_list()
{
    istringstream ss( "foo" );
    ostringstream outss;
    Lexer lexer( ss );
    BracketMatcher matcher( lexer, outss, true );

    TokenList tokens = matcher.ReadCombination();

    TEST_ASSERT_EQUAL( tokens.NextToken().name, "foo" );
    TEST_ASSERT_EQUAL( outss.str(), "" );

    TEST_ASSERT_TRUE( tokens.empty() );
}

void multiple_tokens_yield_multiple_one_item_lists()
{
    istringstream ss( "foo bar" );
    ostringstream outss;
    Lexer lexer( ss );
    BracketMatcher matcher( lexer, outss, true );

    TokenList tokens = matcher.ReadCombination();

    TEST_ASSERT_EQUAL( tokens.NextToken().name, "foo" );
    TEST_ASSERT_EQUAL( outss.str(), "" );
    TEST_ASSERT_TRUE( tokens.empty() );

    tokens = matcher.ReadCombination();

    TEST_ASSERT_EQUAL( tokens.NextToken().name, "bar" );
    TEST_ASSERT_EQUAL( outss.str(), "" );
    TEST_ASSERT_TRUE( tokens.empty() );
}


void combination_yields_all_tokens()
{
    istringstream ss( "(foo bar)" );
    ostringstream outss;
    Lexer lexer( ss );
    BracketMatcher matcher( lexer, outss, true );

    TokenList tokens = matcher.ReadCombination();
    TEST_ASSERT_EQUAL( outss.str(), "" );

    TEST_ASSERT_EQUAL( tokens.NextToken().name, "(" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, "foo" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, "bar" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, ")" );
    TEST_ASSERT_TRUE( tokens.empty() );
}


void nested_combination_yields_all_tokens()
{
    istringstream ss( "(foo (bar) baz) po" );
    ostringstream outss;
    Lexer lexer( ss );
    BracketMatcher matcher( lexer, outss, true );

    TokenList tokens = matcher.ReadCombination();
    TEST_ASSERT_EQUAL( outss.str(), "" );

    TEST_ASSERT_EQUAL( tokens.NextToken().name, "(" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, "foo" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, "(" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, "bar" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, ")" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, "baz" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, ")" );
    TEST_ASSERT_TRUE( tokens.empty() );
}


void incomplete_combo_yields_empty_list()
{
    istringstream ss( "(foo (bar) baz" );
    ostringstream outss;
    Lexer lexer( ss );
    BracketMatcher matcher( lexer, outss, true );

    TokenList tokens = matcher.ReadCombination();

    TEST_ASSERT_EQUAL( outss.str(), "" );
    TEST_ASSERT_TRUE( tokens.empty() );
}



void multiline_combo_yields_all_tokens_and_indents()
{
    istringstream ss( "(foo bar\nbaz)" );
    ostringstream outss;
    Lexer lexer( ss );
    BracketMatcher matcher( lexer, outss, true );

    TokenList tokens = matcher.ReadCombination();
    TEST_ASSERT_EQUAL( outss.str(), ".      " );

    TEST_ASSERT_EQUAL( tokens.NextToken().name, "(" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, "foo" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, "bar" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, "baz" );
    TEST_ASSERT_EQUAL( tokens.NextToken().name, ")" );
    TEST_ASSERT_TRUE( tokens.empty() );
}


void multiline_multicombo_indents()
{
    istringstream ss( "(foo bar ( baz 4 \n)\n)" );
    ostringstream outss;
    Lexer lexer( ss );
    BracketMatcher matcher( lexer, outss, true );

    TokenList tokens = matcher.ReadCombination();
    TEST_ASSERT_EQUAL( outss.str(),
        ".                "
        ".      " );
}



}

void TestBracketMatcher::Run() const
{
    empty_input_yields_empty_list();
    single_token_yields_one_item_list();
    multiple_tokens_yield_multiple_one_item_lists();
    combination_yields_all_tokens();
    nested_combination_yields_all_tokens();
    incomplete_combo_yields_empty_list();
    multiline_combo_yields_all_tokens_and_indents();
    multiline_multicombo_indents();
    // TODO: test passing false for print_prompt
}

