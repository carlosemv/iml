#ifndef __FULL_TYPE_NODE_H__
#define __FULL_TYPE_NODE_H__

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

enum class ExprType
{
    Invalid,
    Integer,
    Bool,
    Float,
    Path,
    Dimensions,
    Section,
    Image,
    Id
};

struct FullType
{
    ExprType type{ExprType::Invalid};
    std::vector<FullType> list_types;

    FullType() = default;
    explicit FullType(ExprType _type);
    [[nodiscard]] std::string to_string() const;
    [[nodiscard]] bool is_num() const;
    [[nodiscard]] bool is_list() const;

    inline friend std::ostream& operator<<(std::ostream& out, const FullType& t)
    {
        return out << t.to_string();
    }
};

#endif
