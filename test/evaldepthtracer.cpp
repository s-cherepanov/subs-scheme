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

#include <cassert>

#include "test/evaldepthtracer.h"

EvalDepthTracer::EvalDepthTracer()
: current_depth_( 0 )
, max_depth_( 0 )
{
}

//virtual
void EvalDepthTracer::IncreaseEvalDepth()
{
    ++current_depth_;
    if( current_depth_ > max_depth_ )
    {
        max_depth_ = current_depth_;
    }
}

//virtual
void EvalDepthTracer::DecreaseEvalDepth()
{
    assert( current_depth_ > 0 );
    --current_depth_;
}

unsigned int EvalDepthTracer::GetMaxEvalDepth()
{
    return max_depth_;
}


