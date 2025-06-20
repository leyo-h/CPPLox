#pragma once
#include "expr.h"
#include "stmt.h"
#include <vector>
#include <memory>

class ASTPrinter : ASTNode{
private:
    string parenthesize(string name, vector<Expr*> exprs);
public:
    std::unique_ptr<Expr> print(std::unique_ptr<Expr> expr);
    void visit(BinaryExpr& binaryExpr) override;
    void visit(LiteralExpr& literalExpr) override;
    void visit(UnaryExpr& unaryExpr) override;
    void visit(GroupingExpr& groupingExpr) override;
    void visit(ExpressionStmt& expressionStmt) override;
    void visit(PrintStmt& printStmt) override;
};