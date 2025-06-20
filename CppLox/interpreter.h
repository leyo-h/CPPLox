#pragma once
#include "expr.h"
#include "lox.h"

class Interpreter : public ASTNode {
private:
    bool isTruthy(const Object& obj);
    bool isEqual(const Object& left, const Object& right);

    //Error handling
    void checkNumberOperand(const Token& token, const Object& right);
    void checkNumberOperands(const Token& token, const Object& left, const Object& right);

    unique_ptr<Stmt> execute(unique_ptr<Stmt> statement);
public:
    virtual ~Interpreter() = default;
    Interpreter();

    std::unique_ptr<vector<unique_ptr<Stmt>>> interpret(std::unique_ptr<vector<unique_ptr<Stmt>>> expr);

    void visit(BinaryExpr& node);
    void visit(LiteralExpr& node);
    void visit(UnaryExpr& node);
    void visit(GroupingExpr& node);
    bool was_error = false;
    std::unique_ptr<Expr> evaluate(std::unique_ptr<Expr> expr);

    //Statements
    void visit(ExpressionStmt& expressionStmt) override;
    void visit(PrintStmt& printStmt) override;

};