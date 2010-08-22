
#include <memory>

#include "combinationvalue.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "valuefactory.h"

using namespace std;

namespace
{

std::auto_ptr<Value> next_value_from_token( Lexer& lexer,
    Token token )
{
    if( token.name.empty() )
    {
        return std::auto_ptr<Value>( NULL );
    }

    if( token.name == "(" )
    {
        auto_ptr<CombinationValue> ret( new CombinationValue );

        while( true )
        {
            token = lexer.NextToken();
            if( token.name == ")" )
            {
                break;
            }
            ret->push_back( next_value_from_token( lexer, token ).release() );
        }
        return auto_ptr<Value>( ret.release() );
    }

    else
    {
        return ValueFactory::CreateValue( token.name );
    }
}


}

Parser::Parser( Lexer& lexer )
: lexer_( lexer )
{
}

std::auto_ptr<Value> Parser::NextValue()
{
    return next_value_from_token( lexer_, lexer_.NextToken() );
}

