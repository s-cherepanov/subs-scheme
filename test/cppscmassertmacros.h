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

#ifndef CPPSCMASSERTMACROS_H
#define CPPSCMASSERTMACROS_H

#include <memory>
#include <string>

class Value;

#define ASSERT_CPPSCM_EQUAL(ARG1,ARG2) \
    cppscmassertfns::equal(__FILE__, __LINE__, #ARG1, #ARG2, ARG1, ARG2)

#define ASSERT_CPPSCM_NOT_EQUAL(ARG1,ARG2) \
    cppscmassertfns::not_equal(__FILE__, __LINE__, #ARG1, #ARG2, ARG1, ARG2)


namespace cppscmassertfns
{


void equal( std::string file, unsigned int line,
    std::string name1, std::string name2,
    const std::auto_ptr<Value>& arg1, const std::auto_ptr<Value>& arg2 );


void not_equal( std::string file, unsigned int line,
    std::string name1, std::string name2,
    const std::auto_ptr<Value>& arg1, const std::auto_ptr<Value>& arg2 );


}


#endif


