#pragma once
//Forward declare the classes
class Expr;
class LiteralExpr;
class UnaryExpr;
class BinaryExpr;
class GroupingExpr;


class Stmt;
class ExpressionStmt;
class PrintStmt;

/*
Implementing "double dispatch" which is our method has to depend on 2 types ( later if its a statement or expression and then on the type?)

*/

using namespace std;
class ASTNode {
private:
public:
    virtual ~ASTNode() = default;
    ASTNode() = default;
    //Visit methods
    virtual void visit(BinaryExpr& node) = 0;
    virtual void visit(LiteralExpr& node) = 0;
    virtual void visit(UnaryExpr& node) = 0;
    virtual void visit(GroupingExpr& node) = 0;

    virtual void visit(ExpressionStmt& node) = 0;
    virtual void visit(PrintStmt& node) = 0;
};