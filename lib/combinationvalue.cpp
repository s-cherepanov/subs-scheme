
#include "combinationvalue.h"

CombinationValue::CombinationValue()
{
}

CombinationValue::CombinationValue( const CombinationValue& other )
{
    for( CombinationValue::const_iterator it = other.begin();
        it != other.end(); ++it )
    {
        push_back( (*it)->Clone() );
    }
}

CombinationValue::~CombinationValue()
{
    for( iterator it = begin(); it != end(); ++it )
    {
        delete *it;
    }
}

//virtual
CombinationValue* CombinationValue::Clone() const
{
    return new CombinationValue( *this );
}

