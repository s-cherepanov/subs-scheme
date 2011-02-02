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

#ifndef DECIMALVALUE_H
#define DECIMALVALUE_H

#include <memory>

#include "lib/value/value.h"

class IntegerValue;

class DecimalValue : public Value
{
public:
    explicit DecimalValue( double value );
    explicit DecimalValue( const IntegerValue& integer_value );
    // Compiler-provided copy constructor is fine

    double GetDoubleValue() const;

    virtual DecimalValue* Clone() const;

    DecimalValue& operator=( const DecimalValue& other );
    DecimalValue& operator=( const IntegerValue& other );

    DecimalValue& operator+=( const IntegerValue& other );
    DecimalValue& operator+=( const DecimalValue& other );
    DecimalValue& operator-=( const IntegerValue& other );
    DecimalValue& operator-=( const DecimalValue& other );
    DecimalValue& operator*=( const IntegerValue& other );
    DecimalValue& operator*=( const DecimalValue& other );
    DecimalValue& operator/=( const IntegerValue& other );
    DecimalValue& operator/=( const DecimalValue& other );

    /**
     * If this decimal value is exactly equal to an integer, return
     * that integer and set succeeded to true.  Otherwise set succeeded to
     * false and return 0.
     */
    int AsExactInt( bool& succeeded ) const;

    friend std::auto_ptr<Value> operator/( const IntegerValue& left,
        const DecimalValue& right );

    friend std::auto_ptr<Value> operator/( const DecimalValue& left,
        const DecimalValue& right );

    friend bool operator==( const DecimalValue& left,
        const DecimalValue& right );

    friend bool operator!=( const DecimalValue& left,
        const DecimalValue& right );

    friend bool operator>=( const IntegerValue& left,
        const DecimalValue& right );

    friend bool operator>=( const DecimalValue& left,
        const IntegerValue& right );

    friend bool operator>=( const DecimalValue& left,
        const DecimalValue& right );

    friend bool operator<=( const IntegerValue& left,
        const DecimalValue& right );

    friend bool operator<=( const DecimalValue& left,
        const IntegerValue& right );

    friend bool operator<=( const DecimalValue& left,
        const DecimalValue& right );

    friend bool operator>( const IntegerValue& left,
        const DecimalValue& right );

    friend bool operator>( const DecimalValue& left,
        const IntegerValue& right );

    friend bool operator>( const DecimalValue& left,
        const DecimalValue& right );

    friend bool operator<( const IntegerValue& left,
        const DecimalValue& right );

    friend bool operator<( const DecimalValue& left,
        const IntegerValue& right );

    friend bool operator<( const DecimalValue& left,
        const DecimalValue& right );

private:
    double value_;
};

#endif

