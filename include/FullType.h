#ifndef __FULL_TYPE_NODE_H__
#define __FULL_TYPE_NODE_H__

#include <iostream>
#include <sstream>
#include <vector>

enum class ExprType
{
    Invalid,
    Integer,
    Float,
    Path,
    Dimensions,
    Section,
    Image,
    Id
};

struct FullType
{
    ExprType type;
    std::vector<FullType> list_types;

    FullType();
    FullType(ExprType _type);
    FullType(const FullType& _ftype);
    std::string to_string() const;

    inline friend std::ostream& operator<<(std::ostream& out, const FullType& t)
    {
        return out << t.to_string();
    }
};

#endif
