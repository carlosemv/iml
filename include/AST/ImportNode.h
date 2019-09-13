#ifndef __IMPORT_NODE_H__
#define __IMPORT_NODE_H__

#include "ExprNode.h"
#include <memory>

class ASTVisitor;
class ImportNode : public ExprNode
{
public:
    ImportNode(Token _t, ExprNode _path);
    ImportNode(Token _t, std::unique_ptr<ExprNode> _path);
    void visit(ASTVisitor& visitor);

    std::unique_ptr<ExprNode> path;
};

#endif
