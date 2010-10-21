
#include <sstream>

#include "lib/value/basic/combinationvalue.h"
#include "lib/argschecker.h"
#include "lib/evaluationerror.h"

using namespace std;

namespace ArgsChecker
{

void CheckExactNumberOfArgs( const char* procedure_name,
    const CombinationValue* args, unsigned int required_number_of_args )
{
    if( args->size() != required_number_of_args )
    {
        ThrowWrongNumArgsException( procedure_name, args->size(),
            required_number_of_args );
    }
}

void ThrowWrongNumArgsException( const char* procedure_name,
    unsigned int actual_number_of_args, unsigned int required_number_of_args )
{
    ostringstream ss;
    if( actual_number_of_args < required_number_of_args )
    {
        ss << "Not enough operands to ";
    }
    else
    {
        ss << "Too many operands to ";
    }
    ss  << procedure_name
        << ": there were "
        << actual_number_of_args
        << " but there should be "
        << required_number_of_args
        << ".";

    throw EvaluationError( ss.str() );
}

void ThrowNotEnoughArgsException( const char* procedure_name,
    unsigned int actual_number_of_args, unsigned int min_number_of_args )
{
    ostringstream ss;
    ss << "Not enough operands to "
        << procedure_name
        << ": there were "
        << actual_number_of_args
        << " but there should be at least "
        << min_number_of_args
        << ".";

    throw EvaluationError( ss.str() );
}

}
