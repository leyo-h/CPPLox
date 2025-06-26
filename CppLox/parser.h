#pragma once
#include "token.h"
#include <vector>
#include <memory>
#include "expr.h"
#include "lox.h"
#include "stmt.h"

class Parser {
private:
    std::unique_ptr<std::vector<unique_ptr<Token>>> tokens;
    int current = 0;

    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();
    std::unique_ptr<Expr> assignment();
    std::unique_ptr<Expr> parseOr();
    std::unique_ptr<Expr> parseAnd();
    std::unique_ptr<Expr> call();
    std::unique_ptr<Expr> finishCall(unique_ptr<Expr> expr);


    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Stmt> printStatement();
    std::unique_ptr<Stmt> expressionStatement();
    std::unique_ptr<Stmt> declaration();
    std::unique_ptr<Stmt> varDeclaration();
    std::unique_ptr<Stmt> ifStatement();
    std::unique_ptr<vector<unique_ptr<Stmt>>> block();
    std::unique_ptr<Stmt> whileStatement();
    std::unique_ptr<Stmt> forStatement();

    //Helpers
    bool match(std::vector<TokenType> types);
    bool check(TokenType type);
    bool isAtEnd();
    //Return indexes into the tokens vector so we dont have to make copies ig :)
    inline int advance();
    inline int peek();
    inline int previous();
    int consume(TokenType type, const char* message);



public:
    Parser(std::unique_ptr<std::vector<unique_ptr<Token>>> setTokens);
    std::unique_ptr<vector<unique_ptr<Stmt>>> parse();

};