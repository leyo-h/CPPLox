#pragma once
#include "token.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory>

//Forward declare the classes
class Expr;
class LiteralExpr;
class UnaryExpr;
class BinaryExpr;
class GroupingExpr;

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
};

class Expr {
private:
public:
    Expr() = default;
    virtual ~Expr() = default;
    virtual void accept(ASTNode& visitor) = 0;

    std::unique_ptr<Object> result;
    string printResult;
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