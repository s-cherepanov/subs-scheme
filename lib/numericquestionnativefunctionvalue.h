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

#ifndef NUMERICQUESTIONNATIVEFUNCTIONVALUE_H
#define NUMERICQUESTIONNATIVEFUNCTIONVALUE_H

#include <memory>
#include <string>

#include "falsevalue.h"
#include "nativefunctionvalue.h"
#include "truevalue.h"

class CombinationValue;
class Environment;

namespace numericquestionutils
{

double get_arg( const char* procedure_name, const CombinationValue* argvalues );

}

template<class QuestionT>
class NumericQuestionNativeFunctionValue : public NativeFunctionValue
{
public:
    virtual std::auto_ptr<Value> Run( const CombinationValue* argvalues ) const
    {
        double value = numericquestionutils::get_arg( StaticName().c_str(),
            argvalues );

        if( QuestionT::Answer( value ) )
        {
            return std::auto_ptr<Value>( new TrueValue );
        }
        else
        {
            return std::auto_ptr<Value>( new FalseValue );
        }
    }

    virtual NumericQuestionNativeFunctionValue* Clone() const
    {
        return new NumericQuestionNativeFunctionValue( *this );
    }

    virtual std::string GetName() const
    {
        return StaticName();
    }

    static const std::string& StaticName()
    {
        return QuestionT::Name();
    }

};



#endif

