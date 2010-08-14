
#ifndef TRACER_H
#define TRACER_H

class Tracer
{
public:
    virtual void IncreaseEvalDepth() = 0;
    virtual void DecreaseEvalDepth() = 0;
};

#endif

