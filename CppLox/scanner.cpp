#include "scanner.h"

/// @brief simple scanner that takes source and atm only removes whitespace to split into tokens
/// @param source string to scan
Scanner::Scanner(std::string source){
    this->source = source;
    this->tokens = std::vector<Token*>();

    this->start = 0;
    this->line = 1;
    this->current = 0;
}

//============== Helper Functions =================

/// @brief Check if we are at the end of the source and there is no more to parse
/// @return Returns true if at the end of the source
bool Scanner::isAtEnd(){
     return this->current >= this->source.length();
}

/// @brief Consume the next character and spit it out!
/// @return the next character in the source
char Scanner::advance(){
    return this->source.at(this->current++);
}

/// @brief Returns the next char in the source but does not consume it!
/// @return the next char in the source or a NULL if we are at the end of the source
char Scanner::peek(){
    if (this->isAtEnd()) return 0;
    return source.at(this->current);
}

/// @brief Add a token to the vector of tokens which has a literal of NULL
/// @param type type of literal
void Scanner::addToken(TokenType type){
    addToken(type, static_cast<Object*>(NULL));
}

/// @brief Add a token to the vector of tokens
/// @param type the type of the token
/// @param literal the literal assosciated with it e.g. string names etc or double values
void Scanner::addToken(TokenType type, Object* literal){
    std::string text = this->source.substr(start, current);
    tokens.push_back(new Token(type, text, literal, line));
}

/// @brief Checks if the next character is expected if it is it consumes it
/// @param expected the character to check for
/// @return true if the next char in the source equal to expected
bool Scanner::match(char expected) {
    if (this->isAtEnd()) return false;
    if (this->source.at(current) != expected) return false;

    this->current++;
    return true;
}

/// @brief incremements current until the next " and adds the string (not including "") as a token
void Scanner::string(){
    while(peek() != '"' && !isAtEnd()){
        if(peek() == '\n') this->line++;
        advance();
    }
    if(isAtEnd()) {
        Lox::error(this->line, "Unterminated String");
        return;
    }

    // Count the closing "
    advance();
    
    //Create the token 
    std::string val = source.substr(start + 1, current - 1);
    addToken(TokenType::STRING, new Object(&val));
}

/// @brief Removes whitespace
/// @return A vector of strings which are the individual tokens
std::vector<Token*> Scanner::scanTokens(){
    while(!this->isAtEnd()){
        this->start = current;
        this->scanToken();
    }

    tokens.push_back(new Token(TokenType::ENDOFFILE, std::string(""),NULL, line));
    return tokens;
}

/// @brief Scans the next token from source and adds it to the vector of tokens
void Scanner::scanToken(){
    char c = advance();
    switch (c) {
        default:
            Lox::error(line, "Unexpected character.");
        break;

        // Single character matches
        case '(': addToken(TokenType::LEFT_PAREN); break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '{': addToken(TokenType::LEFT_BRACE); break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case ',': addToken(TokenType::COMMA); break;
        case '.': addToken(TokenType::DOT); break;
        case '-': addToken(TokenType::MINUS); break;
        case '+': addToken(TokenType::PLUS); break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case '*': addToken(TokenType::STAR); break; 

        //Matches which can be different lexemes if the following character changes
        //E.g. if its ! then the match fails and we do not consume the next character and add a Bang otherwise we add a bang equal!
        case '!':
            addToken(match('=') ? BANG_EQUAL : BANG);
            break;
        case '=':
            addToken(match('=') ? EQUAL_EQUAL : EQUAL);
            break;
        case '<':
            addToken(match('=') ? LESS_EQUAL : LESS);
            break;
        case '>':
            addToken(match('=') ? GREATER_EQUAL : GREATER);
            break;

        //Comments
        case '/':
            if (match('/')) {
                // A comment goes until the end of the line. so keep consuming till we hit the end of the 
                while (peek() != '\n' && !isAtEnd()) advance();
            } else {
                addToken(SLASH);
            }
        break;

        //Whitespace handling
        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace.
            break;
        case '\n':
            line++; //on new line whitespace we need to increase the line
            break;
        
        //Literals
        case '"': 
            string(); 
            break;
    }
}