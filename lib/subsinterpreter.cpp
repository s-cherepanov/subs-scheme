
#include <memory>
#include <string>

#include "evaluator.h"
#include "lexer.h"
#include "parser.h"
#include "prettyprinter.h"
#include "subsinterpreter.h"

using namespace std;

std::string SubsInterpreter::Interpret( const std::string& codestring )
{
    return PrettyPrinter::Print( evaluator_.Eval(
        Parser().Parse(
            Lexer().Lex( codestring ).get() ).get() ).get() );
}

int SubsInterpreter::InterpretStream( std::istream& instream,
    std::ostream& outstream )
{
    Lexer lexer;

    while( instream.good() )
    {
        char buffer[1024];

        bool cont = true;
        
        // Read from the stream until we hit a closing bracket
        while( cont )
        {
            instream.getline( buffer, sizeof( buffer ) );
            cont = !lexer.LexPartial( string( buffer ) );
        }

        string output = PrettyPrinter::Print( evaluator_.Eval( Parser().Parse(
            lexer.GetCombination().get() ).get() ).get() );

        if( !output.empty() )
        {
            outstream << output << endl;
        }
    }

    return 0;
}

void SubsInterpreter::SetTracer( Tracer* tracer )
{
    evaluator_.SetTracer( tracer );
}


