/*
 *  Subs Scheme Interpreter
 *  Copyright (C) 2010 Andy Balaam
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

#include "numericquestionnativefunctionvalue.h"

namespace zeroquestionutils
{

class ZeroQuestion
{
public:
    static bool Answer( double value )
    {
        // TODO: nasty to compare a double against zero!
        return value == 0;
    }

    static const std::string& Name()
    {
        static std::string name( "zero?" );
        return name;
    }
};

}

typedef NumericQuestionNativeFunctionValue<zeroquestionutils::ZeroQuestion>
    ZeroNativeFunctionValue;
