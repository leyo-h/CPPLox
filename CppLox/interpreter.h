#pragma once
#include "expr.h"
#include "lox.h"
#include "environment.h"
#include "clock.h"

class Interpreter : public ASTNode {
private:
    bool isTruthy(const Object& obj);
    bool isEqual(const Object& left, const Object& right);

    //Error handling
    void checkNumberOperand(const Token& token, const Object& right);
    void checkNumberOperands(const Token& token, const Object& left, const Object& right);

    unique_ptr<Stmt> execute(unique_ptr<Stmt> statement);
    shared_ptr<Environment> environment;

    std::unique_ptr<vector<unique_ptr<Stmt>>> executeBlock(std::unique_ptr<vector<unique_ptr<Stmt>>> statements, shared_ptr<Environment> env);
public:
    virtual ~Interpreter() = default;
    Interpreter();

    std::unique_ptr<vector<unique_ptr<Stmt>>> interpret(std::unique_ptr<vector<unique_ptr<Stmt>>> expr);

    void visit(BinaryExpr& node) override;
    void visit(LiteralExpr& node) override;
    void visit(UnaryExpr& node) override;
    void visit(GroupingExpr& node) override;
    void visit(VariableExpr& node) override;
    void visit(AssignExpr& node) override;
    void visit(LogicalExpr& node) override;
    void visit(CallExpr& node) override;


    bool was_error = false;
    std::unique_ptr<Expr> evaluate(std::unique_ptr<Expr> expr);

    //Statements
    void visit(ExpressionStmt& expressionStmt) override;
    void visit(PrintStmt& printStmt) override;
    void visit(VarStmt& node) override;
    void visit(BlockStmt& node) override;
    void visit(IfStmt& node) override;
    void visit(WhileStmt& node) override;
};