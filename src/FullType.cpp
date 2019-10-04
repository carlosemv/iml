#include "FullType.h"

FullType::FullType(ExprType _type) : type(_type)
{}

std::string FullType::to_string() const
{
    std::ostringstream out;
    if (is_list() and not list_types.empty()) {
        out << "(";
        auto it = list_types.begin();
        for (; it != list_types.end(); ++it) {
            out << *it;
            if (it+1 != list_types.end())
                out << ", ";
        }
        out << ")";
    } else {
        out << "<";
        switch (type) {
            case ExprType::Invalid:
                out << "Invalid"; break;
            case ExprType::None:
                out << "None"; break;
            case ExprType::Function:
                out << "Function"; break;
            case ExprType::Integer:
                out << "Int"; break;
            case ExprType::Bool:
                out << "Bool"; break;
            case ExprType::Float:
                out << "Float"; break;
            case ExprType::Path:
                out << "Path"; break;
            case ExprType::Section:
                out << "Section"; break;
            case ExprType::Dimensions:
                out << "Dims"; break;
            case ExprType::Id:
                out << "Id"; break;
            case ExprType::Image:
                out << "Image"; break;
            default:
                out << "???"; break;
        }
        out << ">";
    }
    
    return out.str();
}

bool FullType::is_num() const
{
    return (type == ExprType::Integer
        or type == ExprType::Float);
}

bool FullType::is_list() const
{
    return (type == ExprType::Dimensions
        or type == ExprType::Section);
}
