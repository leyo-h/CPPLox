#include "expr.h"


BinaryExpr::BinaryExpr(std::unique_ptr<Expr> setLeft, std::unique_ptr<Token> setOp,std::unique_ptr<Expr> setRight){
    this->left = std::move(setLeft);
    this->right = std::move(setRight);
    this->op = std::move(setOp);
}

void BinaryExpr::accept(ASTNode& visitor){
    visitor.visit(*this);
}

GroupingExpr::GroupingExpr(std::unique_ptr<Expr> setExpr) {
    this->expr = move(setExpr);
}

void GroupingExpr::accept(ASTNode& visitor){
    visitor.visit(*this);
}

LiteralExpr::LiteralExpr(std::unique_ptr<Object> value) {
    this->value = move(value);
}

void LiteralExpr::accept(ASTNode& visitor){
    visitor.visit(*this);
}

UnaryExpr::UnaryExpr(std::unique_ptr<Token> setOperator, std::unique_ptr<Expr> setRight) {
    this->op = move(setOperator);
    this->right = move(setRight);
}

void UnaryExpr::accept(ASTNode& visitor){
    visitor.visit(*this);
}

void VariableExpr::accept(ASTNode& visitor) {
    visitor.visit(*this);
};

VariableExpr::VariableExpr(std::unique_ptr<Token> setName) {
    name = move(setName);
}