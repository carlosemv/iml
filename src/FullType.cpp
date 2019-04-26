#include "FullType.h"

FullType::FullType() : type(ExprType::Invalid)
{}

FullType::FullType(ExprType _type) : type(_type)
{}

FullType::FullType(const FullType& _ftype) : type(_ftype.type),
    list_types(_ftype.list_types)
{}

std::string FullType::to_string() const
{
    std::ostringstream out;
    if (type != ExprType::Dimensions
        and type != ExprType::Section) {
        out << "<";
        switch (type) {
            case ExprType::Invalid:
                out << "Invalid"; break;
            case ExprType::Integer:
                out << "Int"; break;
            case ExprType::Float:
                out << "Float"; break;
            case ExprType::Path:
                out << "Path"; break;
            case ExprType::Id:
                out << "Id"; break;
            case ExprType::Image:
                out << "Image"; break;
            default:
                out << "???"; break;
        }
        out << ">";
    } else {
        out << "[";
        auto it = list_types.begin();
        for (; it != list_types.end(); ++it) {
            out << *it;
            if (it+1 != list_types.end())
                out << ", ";
        }
        out << "]";
    }
    return out.str();
}