#include "token.h"

/// @brief Create a token. The object is responsible for deleting the literal assosciated with it
/// @param type the token type
/// @param lexeme A pointer to the literal
/// @param literal the literal assosciated with the object if it has it
/// @param line the line the token is on
Token::Token(TokenType type, std::string lexeme, std::unique_ptr<Object>  literal, int line) { 
    this->lexeme = "";
    this->type = type; this->lexeme += lexeme; this->line = line; this->literal = std::move(literal);
};

/// @brief Delete a token and its LITERAL
Token::~Token() {
};

/// @brief Converts a token to the string containing the token type, its lexeme and if possible its literal value
/// @return returns the string
std::string Token::toString() {

    std::string outputStr = std::to_string(this->type) + std::string(" ") + this->lexeme + std::string(" ");
    if (literal != nullptr) outputStr + literal->toString(); // if we dont have no literal then we can convert to string
    return outputStr;
}

std::string Token::getLexme(){
    return this->lexeme;
}

TokenType Token::getType(){
    return this->type;
}


int Token::getLine(){
    return line;
}
