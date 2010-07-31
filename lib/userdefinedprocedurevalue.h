
#ifndef USERDEFINEDPROCEDUREVALUE_H
#define USERDEFINEDPROCEDUREVALUE_H

#include <memory>

#include "procedurevalue.h"

class Evaluator;
class Environment;

/**
 * A procedure defined by the user using lambda or the define
 * syntactic sugar.
 *
 * This class takes ownership of the two CombinationValues you pass to it.
 */
class UserDefinedProcedureValue : public ProcedureValue
{
public:
    UserDefinedProcedureValue( CombinationValue* argnames,
        CombinationValue* body );

    UserDefinedProcedureValue( const UserDefinedProcedureValue& other );

    virtual std::auto_ptr<Value> Run( Evaluator* ev,
        const CombinationValue* combo, Environment& environment );

    virtual UserDefinedProcedureValue* Clone() const;

    virtual std::string GetName() const;

private:
    std::auto_ptr<CombinationValue> argnames_;
    std::auto_ptr<CombinationValue> body_;
};

#endif

