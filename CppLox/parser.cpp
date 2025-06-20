#include "parser.h"
/// @brief Returns true if the next token type is in the vector of token types we give it and advances the parser
/// @param types vector of token types to match the next token against
/// @return Wether or not it was in the vector
bool Parser::match(std::vector<TokenType> types) {
    for (auto type : types){
        if(check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

/// @brief Checks if the next token is of a certain type
/// @param type type to check next token against
/// @return True if the next token matches type
bool Parser::check(TokenType type) {
    if(isAtEnd()) return false;
    return tokens->at(peek())->getType() == type;
}

/// @brief Returns true if we are at the end of the vector of tokens
/// @return true if we are at the end of the tokens to parse
bool Parser::isAtEnd() {
    return tokens->at(peek())->getType() == TokenType::ENDOFFILE;
}

/// @brief Incremenents the current parsing token
/// @return Returns the new token we are parsing
int Parser::advance() {
    if(!isAtEnd()) current++;
    return previous();
}   

/// @brief Returns the current
/// @return self explanatory :)
int Parser::peek() {
    return current;
}
/// @brief Returns the previous token or 0 if there is no previous token
/// @return Returns the previous token or 0 if there is no previous token
int Parser::previous() {

    return max(0,current - 1); // clamp the prev to hit -1 should be fine tho as it would be a parser error if we start with an = etc.
}

std::unique_ptr<Expr> Parser::expression(){
    return equality();
}

std::unique_ptr<Expr> Parser::equality(){
    std::unique_ptr<Expr> expr = comparison();

    while( match({BANG_EQUAL,EQUAL_EQUAL})) {
        std::unique_ptr<Token> op = std::move(tokens->at(previous()));
        //std::printf("consumed %i\n",previous());
        std::unique_ptr<Expr> right = comparison();
        expr = std::make_unique<BinaryExpr>(move(expr),move(op),move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::comparison() {
    std::unique_ptr<Expr> expr = term();

    while(match({GREATER,GREATER_EQUAL,LESS,LESS_EQUAL})){
        std::unique_ptr<Token> op = std::move(tokens->at(previous()));
        //std::printf("consumed %i\n",previous());
        std::unique_ptr<Expr> right = factor();

        expr = std::make_unique<BinaryExpr>(std::move(expr),std::move(op),std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::term() {
    std::unique_ptr<Expr> expr = factor();

    while(match({MINUS, PLUS})){
        std::unique_ptr<Token> op = std::move(tokens->at(previous()));
        //std::printf("consumed %i\n",previous());
        std::unique_ptr<Expr> right = factor();

        expr = std::make_unique<BinaryExpr>(std::move(expr),std::move(op),std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::factor() {
    std::unique_ptr<Expr> expr = unary();

    while(match({SLASH, STAR})){
        std::unique_ptr<Token> op = std::move(tokens->at(previous()));
        //std::printf("consumed %i\n",previous());
        std::unique_ptr<Expr> right = unary();

        expr = std::make_unique<BinaryExpr>(std::move(expr),std::move(op),std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::unary() {
    if(match({TokenType::BANG, TokenType::MINUS})) {
        std::unique_ptr<Token> op = std::move(tokens->at(previous()));
        //std::printf("consumed %i\n",previous());
        std::unique_ptr<Expr> right = unary();

        return std::make_unique<UnaryExpr>(std::move(op),std::move(right));
    }
    return std::move(primary());
}

std::unique_ptr<Expr> Parser::primary() {
    if(match({TokenType::FALSE})) {
        return std::make_unique<LiteralExpr>(std::make_unique<Object>(false));
    }else if(match({TokenType::TRUE})) {
        return std::make_unique<LiteralExpr>(std::make_unique<Object>(true));
    }else if(match({TokenType::NIL})) {
         printf("hmmm nil token thats wrong :(\n");
         return std::make_unique<LiteralExpr>(std::make_unique<Object>());
        }
    else if(match({TokenType::NUMBER,TokenType::STRING})) {
        //std::printf("time to move our token into the new objest\n");
        //std::printf("consuming %i\n",previous());

        return std::make_unique<LiteralExpr>(std::move(tokens->at(previous())->literal));
    }

    if(match({LEFT_PAREN})) {
        std::unique_ptr<Expr> expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression");
        return std::make_unique<GroupingExpr>(std::move(expr));
    }
    printf("Uh oh super invalid primary call! \n\n\n");
    return nullptr;

}

int Parser::consume(TokenType type, const char* message) {
    if(check(type)) return advance();

    Lox::error(tokens->at(peek())->getLine(), message);
    exit(1);
}

Parser::Parser(std::unique_ptr<std::vector<unique_ptr<Token>>> setTokens) {
    tokens = std::move(setTokens);
}

std::unique_ptr<Expr> Parser::parse(){
    return expression();
}