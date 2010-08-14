
#ifndef COMPOUNDPROCEDUREVALUE_H
#define COMPOUNDPROCEDUREVALUE_H

#include <memory>

#include "nativefunctionvalue.h"

class Environment;

/**
 * A procedure defined in Scheme using lambda or the define
 * syntactic sugar.
 *
 * This class takes ownership of the two CombinationValues you pass to it.
 */
 class CompoundProcedureValue : public Value
{
public:
    CompoundProcedureValue( CombinationValue* argnames,
        CombinationValue* body, const std::string& name );

    CompoundProcedureValue( const CompoundProcedureValue& other );

    virtual CompoundProcedureValue* Clone() const;

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

