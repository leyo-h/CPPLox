#pragma once
#include "tokentype.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "object.h"
#include <memory>

class Token {
private:
    TokenType type; // the type of token
    std::string lexeme; //the string of the token
    int line; // the line the token appeaers on
public:
    Token(TokenType type, std::string lexeme, std::unique_ptr<Object>  literal, int line);
    ~Token();
    std::string toString() const;
    std::string getLexme() const;
    TokenType getType() const;
    int getLine() const;

    std::unique_ptr<Object> literal; //idk what this is?
};