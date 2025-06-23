#pragma once
#include "token.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory>
#include "astnode.h"
//Forward declare the classes

typedef enum EXPRTYPE_T {
    BINARY,
    GROUPING,
    LITERAL,
    UNARY,
    VARIABLE,
    ASSIGN,
} EXPRTYPE;


class Expr {
private:
protected:
    EXPRTYPE type = BINARY;
public:
    Expr() = default;
    virtual ~Expr() = default;
    virtual void accept(ASTNode& visitor) = 0;

    std::unique_ptr<Object> result;
    string printResult;
    EXPRTYPE getType();

};

class BinaryExpr : public Expr {
private:

public:
    void accept(ASTNode& visitor) override;
    BinaryExpr(std::unique_ptr<Expr> setLeft, std::unique_ptr<Token> setOp,std::unique_ptr<Expr> setRight);
    unique_ptr<Expr> left;
    unique_ptr<Token> op;
    unique_ptr<Expr> right;
};

class GroupingExpr : public Expr {
private:

public:
    void accept(ASTNode& visitor) override;
    GroupingExpr(std::unique_ptr<Expr> setExpr);
    unique_ptr<Expr> expr;
};


class LiteralExpr : public Expr {
private:

public:
    void accept(ASTNode& visitor) override;
    LiteralExpr(std::unique_ptr<Object> setValue);
    unique_ptr<Object> value;
};


class UnaryExpr : public Expr {
private:

public:
    void accept(ASTNode& visitor) override;
    UnaryExpr(std::unique_ptr<Token> setOperator, std::unique_ptr<Expr> setRight);
    unique_ptr<Token> op;
    unique_ptr<Expr> right;
};

class VariableExpr : public Expr {
private:

public:
    void accept(ASTNode& visitor) override;
    VariableExpr(std::unique_ptr<Token> setName);
    std::unique_ptr<Token> name;
};


class AssignExpr : public Expr {
private:

public:
    void accept(ASTNode& visitor) override;
    AssignExpr(std::unique_ptr<Token> setName, std::unique_ptr<Expr> setValue);
    std::unique_ptr<Token> name;
    std::unique_ptr<Expr> value;
};