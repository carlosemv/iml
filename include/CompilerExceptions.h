#ifndef __COMPILER_EXCEPTIONS_H__
#define __COMPILER_EXCEPTIONS_H__

class CompilerException : public std::logic_error
{
    using std::logic_error::logic_error;
};

class LexicalException : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

class SyntacticException : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

class SemanticException : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

#endif
