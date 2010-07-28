
#include <memory>
#include <string>

#include "lexer.h"
#include "parser.h"
#include "prettyprinter.h"
#include "subsinterpreter.h"

using namespace std;

std::string SubsInterpreter::Interpret( const std::string& codestring ) const
{
    return PrettyPrinter::Print(
        Parser().Parse(
            Lexer().Lex( codestring ).get() ).get() );
}

