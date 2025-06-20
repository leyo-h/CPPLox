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
public:
    virtual ~Interpreter() = default;
    Interpreter();

    std::unique_ptr<Expr> interpret(std::unique_ptr<Expr> expr);

    void visit(BinaryExpr& node);
    void visit(LiteralExpr& node);
    void visit(UnaryExpr& node);
    void visit(GroupingExpr& node);

    std::unique_ptr<Expr> evaluate(std::unique_ptr<Expr> expr);
};