#ifndef __EXPORT_NODE_H__
#define __EXPORT_NODE_H__

#include <memory>
#include "CommandNode.h"
#include "ExprNode.h"

class ASTVisitor;
class ExportNode : public CommandNode
{
private:
    using expr_ptr = std::unique_ptr<ExprNode>;
public:
    ExportNode(Token _t, ExprNode _img, ExprNode _path);
    ExportNode(Token _t, expr_ptr _img, expr_ptr _path);
    void visit(ASTVisitor& visitor);

    expr_ptr image, path;
};

#endif
