#include "tokentype.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "object.h"

class Token {
private:
    TokenType type; // the type of token
    std::string lexeme; //the string of the token
    Object* literal; //idk what this is?
    int line; // the line the token appeaers on
public:
    Token(TokenType type, std::string lexeme, Object* literal, int line);
    ~Token();
    std::string toString();
};