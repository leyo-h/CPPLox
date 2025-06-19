#pragma once
#include "expr.h"
#include <vector>

class ASTPrinter : ASTNode{
private:
    string parenthesize(string name, vector<Expr*> exprs);
public:
    void print(Expr& expr);
    void visit(BinaryExpr& binaryExpr) override;
    void visit(LiteralExpr& literalExpr) override;
    void visit(UnaryExpr& unaryExpr) override;
    void visit(GroupingExpr& groupingExpr) override;
};