
#ifndef WORDLEXER_H
#define WORDLEXER_H

#include <memory>
#include <stack>
#include <string>

class StringTree;
class StringTreeBranch;

class WordLexer
{
public:
    void ProcessWord( const std::string& word );

    // public for test
    std::string HandleOpenBrackets( const std::string& word );

    std::string HandleCloseBrackets( const std::string& word,
        unsigned int& ret_num_stripped );

    void HandleBareSymbol( const std::string symbol );

    std::auto_ptr<StringTree> GetRootTreeItem();

    // public for test
    const std::stack<StringTreeBranch*>& GetTreeStack() { return treestack_; }
private:

    std::auto_ptr<StringTree> root_item_;
    std::stack<StringTreeBranch*> treestack_;

};

#endif

