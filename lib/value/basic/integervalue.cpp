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

#include "lib/value/basic/decimalvalue.h"
#include "lib/value/basic/integervalue.h"

using namespace std;

IntegerValue::IntegerValue( int value )
: value_( value )
{
}

IntegerValue& IntegerValue::operator+=( const IntegerValue& other )
{
    // TODO: handle large integers
    value_ += other.value_;
    return *this;
}


IntegerValue& IntegerValue::operator-=( const IntegerValue& other )
{
    value_ -= other.value_;
    return *this;
}


IntegerValue& IntegerValue::operator*=( const IntegerValue& other )
{
    // TODO: handle large integers
    value_ *= other.value_;
    return *this;
}


int IntegerValue::GetIntValue() const
{
    return value_;
}

//virtual
IntegerValue* IntegerValue::Clone() const
{
    return new IntegerValue( *this );
}


bool operator==( const IntegerValue& left, const IntegerValue& right )
{
    return ( left.value_ == right.value_ );
}

bool operator!=( const IntegerValue& left, const IntegerValue& right )
{
    return ( left.value_ != right.value_ );
}

bool operator>=( const IntegerValue& left, const IntegerValue& right )
{
    return ( left.value_ >= right.value_ );
}

bool operator<=( const IntegerValue& left, const IntegerValue& right )
{
    return ( left.value_ <= right.value_ );
}

bool operator>( const IntegerValue& left, const IntegerValue& right )
{
    return ( left.value_ > right.value_ );
}

bool operator<( const IntegerValue& left, const IntegerValue& right )
{
    return ( left.value_ < right.value_ );
}

std::auto_ptr<Value> operator/( const IntegerValue& left,
    const IntegerValue& right )
{
    if( left.value_ % right.value_ )
    {
        return auto_ptr<Value>( new DecimalValue( static_cast<double>(
            left.value_ ) / static_cast<double>( right.value_ ) ) );
    }
    else
    {
        return auto_ptr<Value>( new IntegerValue( left.value_ / right.value_ )
            );
    }
}

