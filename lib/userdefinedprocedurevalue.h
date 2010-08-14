
#ifndef USERDEFINEDPROCEDUREVALUE_H
#define USERDEFINEDPROCEDUREVALUE_H

#include <memory>

#include "procedurevalue.h"

class Environment;

/**
 * A procedure defined by the user using lambda or the define
 * syntactic sugar.
 *
 * This class takes ownership of the two CombinationValues you pass to it.
 */
 class UserDefinedProcedureValue : public Value
{
public:
    UserDefinedProcedureValue( CombinationValue* argnames,
        CombinationValue* body, const std::string& name );

    UserDefinedProcedureValue( const UserDefinedProcedureValue& other );

    virtual UserDefinedProcedureValue* Clone() const;

    virtual std::string GetName() const;

    const CombinationValue* GetBody() const;

    std::auto_ptr<Environment> ExtendEnvironmentWithArgs(
        const CombinationValue* argvalues, const Environment& environment
        ) const;

private:
    std::auto_ptr<CombinationValue> argnames_;
    std::auto_ptr<CombinationValue> body_;
    std::string name_;
};

#endif

