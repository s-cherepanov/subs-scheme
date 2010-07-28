
#include <string>

class AssertionFailed
{
public:
    AssertionFailed( std::string file, unsigned int line, std::string msg )
    : file_( file )
    , line_( line )
    , msg_( msg )
    {
    }

    std::string ToString();

private:
    std::string file_;
    unsigned int line_;
    std::string msg_;
};

