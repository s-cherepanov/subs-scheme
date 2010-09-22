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

#ifndef ARGSCHECKER_H
#define ARGSCHECKER_H

class CombinationValue;

namespace ArgsChecker
{

/**
 * Check that the supplied combo has the size provided,
 * and throw an EvaluationError if not.
 */
void CheckExactNumberOfArgs( const char* procedure_name,
    const CombinationValue* args, unsigned int required_number_of_args );

void ThrowWrongNumArgsException( const char* procedure_name,
    unsigned int actual_number_of_args, unsigned int required_number_of_args );

void ThrowNotEnoughArgsException( const char* procedure_name,
    unsigned int actual_number_of_args, unsigned int min_number_of_args );

}

#endif

