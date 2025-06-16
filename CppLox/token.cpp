#include "token.h"

Token::Token(TokenType type, std::string lexeme, Object* literal, int line) { 
    this->type = type; this->lexeme = lexeme; this->line = line; this->literal = literal;
};

Token::~Token() {
    delete literal;
};

std::string Token::toString() {

    std::string outputStr = std::to_string(this->type) + std::string(" ") + this->lexeme + std::string(" ");
    if (literal != NULL) outputStr + literal->toString(); // if we dont have no literal then we can convert to string
    return outputStr;
}