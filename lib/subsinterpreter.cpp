
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

