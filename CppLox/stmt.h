#pragma once
#include "astnode.h"
#include <memory>
#include <string>
#include "object.h"
#include "expr.h"

using namespace std;
class Stmt {
private:
public:
    Stmt() = default;
    virtual ~Stmt() = default;
    virtual void accept(ASTNode& visitor) = 0;

    std::unique_ptr<Object> result;
    string printResult;
};

class ExpressionStmt : public Stmt {
private:

public:
    void accept(ASTNode& visitor) override;
    ExpressionStmt(std::unique_ptr<Expr> setExpression);
    unique_ptr<Expr> expression;
};

class PrintStmt : public Stmt {
private:

public:
    void accept(ASTNode& visitor) override;
    PrintStmt(std::unique_ptr<Expr> setExpression);
    unique_ptr<Expr> expression;
};

class VarStmt : public Stmt {
private:

public:
    void accept(ASTNode& visitor) override;
    VarStmt(unique_ptr<Token> name, unique_ptr<Expr> initialiser);
    unique_ptr<Token> name;
    unique_ptr<Expr> initialiser;
};

