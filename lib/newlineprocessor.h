
#ifndef NEWLINEPROCESSOR_H
#define NEWLINEPROCESSOR_H

#include <iosfwd>
#include <stack>

class NewLineProcessor
{
public:
    NewLineProcessor( std::ostream& out );

    void PushIndent( unsigned int level, unsigned int indent );
    void PopIndent( unsigned int level );

    unsigned int GetIndent() const;

    void NewLine();

    void Reset();

private:
    std::ostream& out_;
    std::stack< std::pair<unsigned int, unsigned int> > indent_stack_;
    unsigned int indent_;
};

#endif

