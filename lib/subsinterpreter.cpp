
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "evaluator.h"
#include "lexer.h"
#include "parser.h"
#include "prettyprinter.h"
#include "subsinterpreter.h"
#include "tokenlist.h"

using namespace std;

std::string SubsInterpreter::Interpret( const std::string& codestring )
{
    istringstream ss( codestring );
    Lexer lexer( ss );
    Parser parser( lexer );

    return PrettyPrinter::Print( evaluator_.Eval( parser.NextValue().get()
        ).get() );
}

std::string SubsInterpreter::InterpretTokens( TokenList& tokens )
{
    Parser parser( tokens );

    return PrettyPrinter::Print( evaluator_.Eval( parser.NextValue().get()
        ).get() );
}

int SubsInterpreter::InterpretStream( std::istream& instream,
    std::ostream& outstream )
{
    Lexer lexer( instream );
    Parser parser( lexer );

    auto_ptr<Value> value = parser.NextValue();
    while( value.get() )
    {
        string output = PrettyPrinter::Print( evaluator_.Eval( value.get()
            ).get() );

        if( !output.empty() )
        {
            outstream << output << endl;
        }

        value = parser.NextValue();
    }

    // TODO: handle parse errors

    return 0;
}

void SubsInterpreter::SetTracer( Tracer* tracer )
{
    evaluator_.SetTracer( tracer );
}


