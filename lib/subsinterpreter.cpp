
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "evaluator.h"
#include "newlexer.h"
#include "newparser.h"
#include "prettyprinter.h"
#include "subsinterpreter.h"

using namespace std;

std::string SubsInterpreter::Interpret( const std::string& codestring )
{
    istringstream ss( codestring );
    NewLexer lexer( ss );
    NewParser parser( lexer );

    return PrettyPrinter::Print( evaluator_.Eval( parser.NextValue().get()
        ).get() );
}

int SubsInterpreter::InterpretStream( std::istream& instream,
    std::ostream& outstream )
{
    NewLexer lexer( instream );
    NewParser parser( lexer );

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


