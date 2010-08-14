
#include <cassert>

#include "evaldepthtracer.h"

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


