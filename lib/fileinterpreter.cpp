
#include <string>

#include "fileinterpreter.h"

FileInterpreter::FileInterpreter( const std::string& filename )
: file_( filename.c_str() )
{
    // TODO: handle missing files etc.
}

int FileInterpreter::Interpret( std::ostream& outstream )
{
    return interpreter_.InterpretStream( file_, outstream );
}

