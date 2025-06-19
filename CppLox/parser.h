#pragma once
#include "token.h"
#include <vector>
#include <memory>
#include "expr.h"
#include "lox.h"

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
    std::unique_ptr<Expr> parse();

};