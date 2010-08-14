
#ifndef NULLTRACER_H
#define NULLTRACER_H

#include "tracer.h"

class NullTracer : public Tracer
{
    virtual void IncreaseEvalDepth();
    virtual void DecreaseEvalDepth();
};

#endif

