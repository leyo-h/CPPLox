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