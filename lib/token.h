/*
 *  Subs Scheme Interpreter
 *  Copyright (C) 2010, 2011 Andy Balaam
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**/

#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token
{
public:
    enum EType
    {
          eTypeNormal
        , eTypeString
    };

    Token( char name, Token::EType type, unsigned int column );
    Token( const std::string& name, Token::EType type, unsigned int column );

    const std::string& Name() const;
    Token::EType Type() const;
    unsigned int Column() const;

    bool IsEndOfStream() const;

    void AddToColumn( unsigned int adjustment );

private:
    std::string name_;
    Token::EType type_;

    //TODO: std::string filename;
    //TODO: unsigned int line_number;
    unsigned int column_;
};

#endif

