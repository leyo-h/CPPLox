#include "expr.h"

EXPRTYPE Expr::getType(){
    return type;
}


BinaryExpr::BinaryExpr(std::unique_ptr<Expr> setLeft, std::unique_ptr<Token> setOp,std::unique_ptr<Expr> setRight){
    this->left = std::move(setLeft);
    this->right = std::move(setRight);
    this->op = std::move(setOp);
    this->type = BINARY;
}

void BinaryExpr::accept(ASTNode& visitor){
    visitor.visit(*this);
}

GroupingExpr::GroupingExpr(std::unique_ptr<Expr> setExpr) {
    this->expr = move(setExpr);
    this->type = GROUPING;

}

void GroupingExpr::accept(ASTNode& visitor){
    visitor.visit(*this);
}

LiteralExpr::LiteralExpr(std::unique_ptr<Object> value) {
    this->value = move(value);
    this->type = LITERAL;

}

void LiteralExpr::accept(ASTNode& visitor){
    visitor.visit(*this);
}

UnaryExpr::UnaryExpr(std::unique_ptr<Token> setOperator, std::unique_ptr<Expr> setRight) {
    this->op = move(setOperator);
    this->right = move(setRight);
    this->type = UNARY;

}

void UnaryExpr::accept(ASTNode& visitor){
    visitor.visit(*this);
}

void VariableExpr::accept(ASTNode& visitor) {
    visitor.visit(*this);
};

VariableExpr::VariableExpr(std::unique_ptr<Token> setName) {
    name = move(setName);
    this->type = VARIABLE;
}

void AssignExpr::accept(ASTNode& visitor) {
    visitor.visit(*this);
}

AssignExpr::AssignExpr(std::unique_ptr<Token> setName, std::unique_ptr<Expr> setValue) {
    name = move(setName);
    value = move(setValue);
    this->type = ASSIGN;
}