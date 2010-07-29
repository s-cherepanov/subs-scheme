
#include <cassert>
#include <stack>
#include <string>

#include "stringtree.h"
#include "stringtreebranch.h"
#include "stringtreeleaf.h"

#include "wordlexer.h"

using namespace std;

void WordLexer::ProcessWord( const std::string& word )
{
    string symbol = HandleOpenBrackets( word );

    unsigned int num_to_pop = 0;
    symbol = HandleCloseBrackets( symbol, num_to_pop );

    HandleBareSymbol( symbol );

    for( unsigned int i = 0; i < num_to_pop; ++i )
    {
        treestack_.pop();
    }
}

std::string WordLexer::HandleOpenBrackets( const std::string& word )
{
    std::string symbol = word;

    while( !symbol.empty() && *symbol.begin() == '(' )
    {
        auto_ptr<StringTreeBranch> new_branch( new StringTreeBranch );
        StringTreeBranch* new_branch_p = new_branch.get();
        if( !treestack_.empty() )
        {
            treestack_.top()->push_back( new_branch.release() );
            treestack_.push( new_branch_p );
        }
        else
        {
            assert( !root_item_.get() );
            root_item_ = new_branch;
            treestack_.push( new_branch_p );
        }
        symbol = symbol.substr( 1 );
    }

    return symbol;
}


std::string WordLexer::HandleCloseBrackets( const std::string& word,
        unsigned int& ret_num_stripped )
{
    string symbol( word );

    ret_num_stripped = 0;
    while( !symbol.empty() && *symbol.rbegin() == ')' )
    {
        ++ret_num_stripped;
        symbol = symbol.substr( 0, symbol.size() - 1 );
    }

    return symbol;
}

void WordLexer::HandleBareSymbol( const std::string symbol )
{
    if( !symbol.empty() )
    {
        auto_ptr<StringTreeLeaf> new_leaf( new StringTreeLeaf( symbol ) );
        if( !treestack_.empty() )
        {
            treestack_.top()->push_back( new_leaf.release() );
        }
        else
        {
            assert( !root_item_.get() );
            root_item_ = new_leaf;
        }
    }

}

std::auto_ptr<StringTree> WordLexer::GetRootTreeItem()
{
    // TODO: throw something if lexing fails instead of crashing

    assert( treestack_.empty() );
    assert( root_item_.get() );

    return root_item_;
}

