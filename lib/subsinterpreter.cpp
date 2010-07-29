
#include <memory>
#include <string>

#include "evaluator.h"
#include "lexer.h"
#include "parser.h"
#include "prettyprinter.h"
#include "subsinterpreter.h"

using namespace std;

std::string SubsInterpreter::Interpret( const std::string& codestring ) const
{
    auto_ptr<Value> value = Parser().Parse( Lexer().Lex( codestring ).get() );

    auto_ptr<Value> new_value = Evaluator().Eval( value.get() );
    if( new_value.get() )
    {
        value = new_value;
    }

    return PrettyPrinter::Print( value.get() );
}

