
#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <memory>
#include <string>

class StringTree;

class Lexer
{
public:
    std::auto_ptr<StringTree> Lex( const std::string& codestring ) const;

    /**
     * Lex the next bit of string.  If we didn't complete a tree,
     * remember what strings we have been passed and add any further calls
     * to this method on to them.
     *
     * @return true if the supplied string gets us to the end of a tree.
     */
    bool LexPartial( const std::string& next_string );

    /**
     * When LexPartial returns true, call this method to get the lexed
     * tree back and reset the stored state so only parts of string that
     * have not been included in the lexed tree remain.
     */
    std::auto_ptr<StringTree> GetCombination();

private:
    std::string partial_;
    std::auto_ptr<StringTree> lexed_tree_;
};

#endif

