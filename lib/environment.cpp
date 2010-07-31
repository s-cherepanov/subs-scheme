
#include <map>

#include "value.h"

#include "environment.h"

using namespace std;

Environment::Environment()
: extended_parent_( NULL )
{
}

Environment::Environment( const Environment& parent_to_extend, bool )
: extended_parent_( &parent_to_extend )
{
}

Environment::~Environment()
{
    for( MapType::iterator it = symbols_.begin(); it != symbols_.end(); ++it )
    {
        delete it->second;
    }
}

void Environment::InsertSymbol( const std::string& name, Value* value )
{
    symbols_[name] = value;
}

const Value* Environment::FindSymbol( const std::string& name ) const
{
    MapType::const_iterator itFind = symbols_.find( name );
    if( itFind != symbols_.end() )
    {
        return itFind->second;
    }
    else if( extended_parent_ )
    {
        return extended_parent_->FindSymbol( name );
    }
    else
    {
        return NULL;
    }
}

