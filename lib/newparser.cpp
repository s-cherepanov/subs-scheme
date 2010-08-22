
#include <memory>

#include "combinationvalue.h"
#include "newlexer.h"
#include "newparser.h"
#include "token.h"
#include "valuefactory.h"

using namespace std;

namespace
{

std::auto_ptr<Value> next_value_from_token( NewLexer& lexer,
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

NewParser::NewParser( NewLexer& lexer )
: lexer_( lexer )
{
}

std::auto_ptr<Value> NewParser::NextValue()
{
    return next_value_from_token( lexer_, lexer_.NextToken() );
}

