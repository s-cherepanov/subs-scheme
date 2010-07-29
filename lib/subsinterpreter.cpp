
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
    auto_ptr<Value> value = Parser().Parse( Lexer().Lex( codestring ).get() );

    // value will be null if the string was empty.
    if( value.get() )
    {
        value = evaluator_.Eval( value.get() );
    }

    return PrettyPrinter::Print( value.get() );
}

