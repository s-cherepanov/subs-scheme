
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <map>
#include <string>

class Value;

/**
 * A map of symbol names to their values, which owns the values.
 */
class Environment
{
public:
    /**
     * Construct a top-level environment.
     */
    Environment();

    // Note compiler-provided copy constructor exists and performs
    // a shallow copy.

    /**
     * Create an environment by extending another.
     * The second argument is a dummy to differentiate this from
     * a copy constructor.
     */
    Environment( const Environment& parent_to_extend, bool );

    ~Environment();

    /**
     * Add a symbol to this environment.  This object takes ownership
     * of the provided Value object and will delete when it is no
     * longer needed.
     */
    void InsertSymbol( const std::string& name, Value* value );

    const Value* FindSymbol( const std::string& name ) const;

private:

    typedef std::map<std::string, Value*> MapType;
    MapType symbols_;
    const Environment* extended_parent_;
};

#endif

