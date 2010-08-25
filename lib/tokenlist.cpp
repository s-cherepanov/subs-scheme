
#include <vector>

#include "token.h"
#include "tokenlist.h"

void TokenList::AddToken( const Token& token )
{
    list_.push( token );
}

//virtual
Token TokenList::NextToken()
{
    if( list_.empty() )
    {
        return Token();
    }

    Token ret = list_.front();
    list_.pop();
    return ret;
}


bool TokenList::empty() const
{
    return list_.empty();
}

