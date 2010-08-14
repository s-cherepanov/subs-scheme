
#ifndef EVALDEPTHTRACER_H
#define EVALDEPTHTRACER_H

#include "lib/tracer.h"

class EvalDepthTracer : public Tracer
{
public:

    EvalDepthTracer();

    virtual void IncreaseEvalDepth();
    virtual void DecreaseEvalDepth();

    unsigned int GetMaxEvalDepth();

private:
    unsigned int current_depth_;
    unsigned int max_depth_;
};

#endif

