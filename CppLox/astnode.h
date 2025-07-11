#pragma once
//Forward declare the classes
class Expr;
class LiteralExpr;
class UnaryExpr;
class BinaryExpr;
class GroupingExpr;
class VariableExpr;
class AssignExpr;
class LogicalExpr;
class CallExpr;

class Stmt;
class ExpressionStmt;
class PrintStmt;
class VarStmt;
class BlockStmt;
class IfStmt;
class WhileStmt;
class FunctionStmt;
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
    virtual void visit(VariableExpr& node) = 0;
    virtual void visit(AssignExpr& node) = 0;
    virtual void visit(LogicalExpr& node) = 0;
    virtual void visit(CallExpr& node) = 0;


    virtual void visit(ExpressionStmt& node) = 0;
    virtual void visit(PrintStmt& node) = 0;
    virtual void visit(VarStmt& node) = 0;
    virtual void visit(BlockStmt& node) = 0;
    virtual void visit(IfStmt& node) = 0;
    virtual void visit(WhileStmt& node) = 0;
    virtual void visit(FunctionStmt& node) = 0;

};