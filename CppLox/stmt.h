#pragma once
#include "astnode.h"
#include <memory>
#include <string>
#include "object.h"
#include "expr.h"
#include <vector>

using namespace std;

typedef enum STMTTYPE_T {
    EXPRESSIONSTMT,
    PRINTSTMT,
    VARSTMT,
    BLOCKSTMT,
    IFSTMT,
    WHILESTMT,
    FUNCTIONSTMT,
} STMTTYPE;

class Stmt {
private:
public:
    Stmt() = default;
    virtual ~Stmt() = default;
    virtual void accept(ASTNode& visitor) = 0;

    std::unique_ptr<Object> result;
    string printResult;
    STMTTYPE type;
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

class BlockStmt : public Stmt {
private:

protected:

public:
    BlockStmt(unique_ptr<std::vector<unique_ptr<Stmt>>> setStatements);
    void accept(ASTNode& visitor) override;
    unique_ptr<std::vector<unique_ptr<Stmt>>> statements;
};

class IfStmt : public Stmt {
private:

protected:

public:
    IfStmt(unique_ptr<Expr> setCondition, unique_ptr<Stmt> setThenBranch, unique_ptr<Stmt> setElseBranch);
    void accept(ASTNode& visitor) override;
    unique_ptr<Expr> condition;
    unique_ptr<Stmt> thenBranch;
    unique_ptr<Stmt> elseBranch;

};

class WhileStmt : public Stmt {
private:

protected:

public:
    WhileStmt(unique_ptr<Expr> setCondition, unique_ptr<Stmt> setBody);
    void accept(ASTNode& visitor) override;
    unique_ptr<Expr> condition;
    unique_ptr<Stmt> body;


};


class FunctionStmt : public Stmt {
private:

protected:

public:
    FunctionStmt(unique_ptr<Token> setName, unique_ptr<vector<unique_ptr<Token>>> setParams, unique_ptr<vector<unique_ptr<Stmt>>> setBody);
    void accept(ASTNode& visitor) override;
    unique_ptr<Token> name;
    unique_ptr<vector<unique_ptr<Token>>> params;
    unique_ptr<vector<unique_ptr<Stmt>>> body;


};


