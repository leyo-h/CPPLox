#include "stmt.h"



void ExpressionStmt::accept(ASTNode& visitor){
    visitor.visit(*this);
};
ExpressionStmt::ExpressionStmt(std::unique_ptr<Expr> setExpression){
    expression = std::move(setExpression);
}


void PrintStmt::accept(ASTNode& visitor) {
    visitor.visit(*this);
}
PrintStmt::PrintStmt(std::unique_ptr<Expr> setExpression){
    expression = std::move(setExpression);
}


void VarStmt::accept(ASTNode& visitor) {
    visitor.visit(*this);
}


VarStmt::VarStmt(unique_ptr<Token> setName, unique_ptr<Expr> setInitialiser){
    name = move(setName);
    initialiser = move(setInitialiser);

}

BlockStmt::BlockStmt(unique_ptr<std::vector<unique_ptr<Stmt>>> setStatements) {
    statements = move(setStatements);
}

void BlockStmt::accept(ASTNode& visitor) {
    visitor.visit(*this);
}


IfStmt::IfStmt(unique_ptr<Expr> setCondition, unique_ptr<Stmt> setThenBranch, unique_ptr<Stmt> setElseBranch) {
    condition = move(setCondition);
    thenBranch = move(setThenBranch);
    elseBranch = move(setElseBranch);

}

void IfStmt::accept(ASTNode& visitor) {
    visitor.visit(*this);
}