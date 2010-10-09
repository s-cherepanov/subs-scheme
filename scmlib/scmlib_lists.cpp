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

#include <string>

#include "scmlib_lists.h"

const char** scmlib_lists()
{
    static const char* ret[] =
    {
        "(define (map fn list) "
                "(if (null? list) "
                    "nil "
                    "(cons (fn (car list)) "
                          "(map fn (cdr list))))) ",

        "(define (length list) "
                "(define (length-iter total list) "
                        "(if (null? list) "
                            "total "
                            "(length-iter (+ total 1) (cdr list)))) "
                "(length-iter 0 list)) ",

        "(define (append list1 list2) "
                "(if (null? list1) "
                    "list2 "
                    "(cons (car list1) (append (cdr list1) list2)))) ",

        "(define (list-ref list n) "
                "(if (= n 0) "
                    "(car list) "
                    "(list-ref (cdr list) (- n 1)))) ",

        ""
    };

    return ret;
}
