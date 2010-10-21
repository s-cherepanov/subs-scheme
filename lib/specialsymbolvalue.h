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

#ifndef SPECIALSYMBOLVALUE_H
#define SPECIALSYMBOLVALUE_H

#include <string>

#include "symbolvalue.h"

class SpecialSymbolValue : public SymbolValue
{
public:

    enum SymbolType
    {
          t_CUSTOM = -1
        , t_and
        , t_car
        , t_cdr
        , t_cond
        , t_cons
        , t_define
        , t_display
        , t_else
        , t_if
        , t_lambda
        , t_let
        , t_list
        , t_newline
        , t_or
    };

    SpecialSymbolValue( SymbolType symbol_type );

    virtual const std::string& GetStringValue() const;

    virtual SpecialSymbolValue* Clone() const;

    SpecialSymbolValue::SymbolType GetSymbolType() const;

    static SpecialSymbolValue::SymbolType String2SymbolType(
        const std::string& symbol_name );

private:
    SymbolType symbol_type_;
};

#endif
