
#include <memory>
#include <sstream>
#include <string>

#include "emptystringtree.h"
#include "stringtree.h"
#include "stringtreebranch.h"
#include "stringtreeleaf.h"
#include "wordlexer.h"

#include "lexer.h"

using namespace std;

std::auto_ptr<StringTree> Lexer::Lex( const std::string& codestring ) const
{
    // TODO: handle anything other than a single symbol

    if( codestring.empty() )
    {
        return auto_ptr<StringTree>( new EmptyStringTree() );
    }

    istringstream codestream( codestring );
    string symbol;

    WordLexer wordlexer;

    while( codestream.good() )
    {
        codestream >> symbol;
        wordlexer.ProcessWord( symbol );
    }

    return wordlexer.GetRootTreeItem();
}

