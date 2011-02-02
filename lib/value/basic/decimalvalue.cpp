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

#include <memory>

#include "lib/value/basic/decimalvalue.h"
#include "lib/value/basic/integervalue.h"

using namespace std;

//explicit
DecimalValue::DecimalValue( double value )
: value_( value )
{
}

//explicit
DecimalValue::DecimalValue( const IntegerValue& integer_value )
: value_( static_cast<double>( integer_value.GetIntValue() ) )
{
}


double DecimalValue::GetDoubleValue() const
{
    return value_;
}

//virtual
DecimalValue* DecimalValue::Clone() const
{
    return new DecimalValue( *this );
}

DecimalValue& DecimalValue::operator=( const DecimalValue& other )
{
    value_ = other.value_;
    return *this;
}

DecimalValue& DecimalValue::operator=( const IntegerValue& other )
{
    value_ = other.GetIntValue();
    return *this;
}

DecimalValue& DecimalValue::operator+=( const IntegerValue& other )
{
    value_ += static_cast<double>( other.GetIntValue() );
    return *this;
}

DecimalValue& DecimalValue::operator+=( const DecimalValue& other )
{
    value_ += other.value_;
    return *this;
}

DecimalValue& DecimalValue::operator-=( const IntegerValue& other )
{
    value_ -= static_cast<double>( other.GetIntValue() );
    return *this;
}

DecimalValue& DecimalValue::operator-=( const DecimalValue& other )
{
    value_ -= other.value_;
    return *this;
}


DecimalValue& DecimalValue::operator*=( const IntegerValue& other )
{
    value_ *= static_cast<double>( other.GetIntValue() );
    return *this;
}

DecimalValue& DecimalValue::operator*=( const DecimalValue& other )
{
    value_ *= other.value_;
    return *this;
}

DecimalValue& DecimalValue::operator/=( const IntegerValue& other )
{
    value_ /= static_cast<double>( other.GetIntValue() );
    return *this;
}

DecimalValue& DecimalValue::operator/=( const DecimalValue& other )
{
    value_ /= other.value_;
    return *this;
}


int DecimalValue::AsExactInt( bool& succeeded ) const
{
    int ans = static_cast<int>( value_ );
    if( ans == value_ )
    {
        succeeded = true;
        return ans;
    }
    else
    {
        succeeded = false;
        return 0;
    }
}


std::auto_ptr<Value> operator/( const IntegerValue& left,
    const DecimalValue& right )
{
    return auto_ptr<Value>( new DecimalValue(
        static_cast<double>( left.GetIntValue() ) / right.GetDoubleValue() ) );
}

std::auto_ptr<Value> operator/( const DecimalValue& left,
    const DecimalValue& right )
{
    return auto_ptr<Value>( new DecimalValue( left.GetDoubleValue() /
        right.GetDoubleValue() ) );
}



bool operator==( const DecimalValue& left, const DecimalValue& right )
{
    return ( left.value_ == right.value_ );
}

bool operator!=( const DecimalValue& left, const DecimalValue& right )
{
    return ( left.value_ != right.value_ );
}


bool operator>=( const IntegerValue& left, const DecimalValue& right )
{
    return ( left.GetIntValue() >= right.value_ );
}

bool operator>=( const DecimalValue& left, const IntegerValue& right )
{
    return ( left.value_ >= right.GetIntValue() );
}

bool operator>=( const DecimalValue& left, const DecimalValue& right )
{
    return ( left.value_ >= right.value_ );
}

bool operator<=( const IntegerValue& left, const DecimalValue& right )
{
    return ( left.GetIntValue() <= right.value_ );
}

bool operator<=( const DecimalValue& left, const IntegerValue& right )
{
    return ( left.value_ <= right.GetIntValue() );
}

bool operator<=( const DecimalValue& left, const DecimalValue& right )
{
    return ( left.value_ <= right.value_ );
}



bool operator>( const IntegerValue& left, const DecimalValue& right )
{
    return ( left.GetIntValue() > right.value_ );
}

bool operator>( const DecimalValue& left, const IntegerValue& right )
{
    return ( left.value_ > right.GetIntValue() );
}

bool operator>( const DecimalValue& left, const DecimalValue& right )
{
    return ( left.value_ > right.value_ );
}

bool operator<( const IntegerValue& left, const DecimalValue& right )
{
    return ( left.GetIntValue() < right.value_ );
}

bool operator<( const DecimalValue& left, const IntegerValue& right )
{
    return ( left.value_ < right.GetIntValue() );
}

bool operator<( const DecimalValue& left, const DecimalValue& right )
{
    return ( left.value_ < right.value_ );
}



