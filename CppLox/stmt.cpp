#include "stmt.h"



void ExpressionStmt::accept(ASTNode& visitor){
    visitor.visit(*this);
};
ExpressionStmt::ExpressionStmt(std::unique_ptr<Expr> setExpression){
    expression = std::move(setExpression);
    type = EXPRESSIONSTMT;
}


void PrintStmt::accept(ASTNode& visitor) {
    visitor.visit(*this);
}
PrintStmt::PrintStmt(std::unique_ptr<Expr> setExpression){
    expression = std::move(setExpression);
    type = PRINTSTMT;
}


void VarStmt::accept(ASTNode& visitor) {
    visitor.visit(*this);
}


VarStmt::VarStmt(unique_ptr<Token> setName, unique_ptr<Expr> setInitialiser){
    name = move(setName);
    initialiser = move(setInitialiser);
    type = VARSTMT;
}

BlockStmt::BlockStmt(unique_ptr<std::vector<unique_ptr<Stmt>>> setStatements) {
    statements = move(setStatements);
    type = BLOCKSTMT;
}

void BlockStmt::accept(ASTNode& visitor) {
    visitor.visit(*this);
}


IfStmt::IfStmt(unique_ptr<Expr> setCondition, unique_ptr<Stmt> setThenBranch, unique_ptr<Stmt> setElseBranch) {
    condition = move(setCondition);
    thenBranch = move(setThenBranch);
    elseBranch = move(setElseBranch);
    type = IFSTMT;
}

void IfStmt::accept(ASTNode& visitor) {
    visitor.visit(*this);
}

WhileStmt::WhileStmt(unique_ptr<Expr> setCondition, unique_ptr<Stmt> setBody) {
    condition = move(setCondition);
    body = move(setBody);
    type = WHILESTMT;
}

void WhileStmt::accept(ASTNode& visitor) {
    visitor.visit(*this);
}

FunctionStmt::FunctionStmt(unique_ptr<Token> setName, unique_ptr<vector<unique_ptr<Token>>> setParams, unique_ptr<vector<unique_ptr<Stmt>>> setBody) {
    name= move(setName);
    body = move(setBody);
    params = move(setParams);
    type = FUNCTIONSTMT;
}
void FunctionStmt::accept(ASTNode& visitor) {
    visitor.visit(*this);
}