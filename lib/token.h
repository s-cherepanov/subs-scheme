
#ifndef TOKEN_H
#define TOKEN_H

#include <string>

struct Token
{
    std::string name;

    std::string filename;
    unsigned int line_number;
    unsigned int column_number;
};

#endif

