
#ifndef TOKEN_H
#define TOKEN_H

#include <string>

struct Token
{
    std::string name;

    //TODO: std::string filename;
    //TODO: unsigned int line_number;
    unsigned int column;
};

#endif

