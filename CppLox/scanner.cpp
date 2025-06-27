#include "scanner.h"

/// @brief simple scanner that takes source and atm only removes whitespace to split into tokens
/// @param source string to scan
Scanner::Scanner(std::string source) {
    this->source = source;
    this->tokens = std::make_unique<std::vector<std::unique_ptr<Token>>>();

    this->start = 0;
    this->line = 1;
    this->current = 0;

    initKeywordsMap();
}

//============== Helper Functions =================

/// @brief Check if we are at the end of the source and there is no more to parse
/// @return Returns true if at the end of the source
bool Scanner::isAtEnd() {
     return this->current >= this->source.length();
}

/// @brief Consume the next character and spit it out!
/// @return the next character in the source
char Scanner::advance() {
    return this->source.at(this->current++);
}

/// @brief Returns the next char in the source but does not consume it!
/// @return the next char in the source or a NULL if we are at the end of the source
char Scanner::peek() {
    if (this->isAtEnd()) return 0;
    return source.at(this->current);
}

/// @brief Returns the char following the next char but does not consume it
/// @return Returns the char unless we are OOB then we return 0
char Scanner::peekNext() {
    if (current + 1 >= source.length()) return 0;
    return source.at(this->current + 1);
}

void Scanner::advanceMultilineComment(){
    while(!isAtEnd()){
        switch (peek()) { 
            case '\n':
                this->line++;
                break;
            case '*':
                if (peekNext() == '/'){
                    advance(); // consume *
                    advance(); // consume /
                    return;
                }
                break;
            default:
                break;
        }
        advance();
    }
}

/// @brief Add a token to the vector of tokens which has a literal of NULL
/// @param type type of literal
void Scanner::addToken(TokenType type){
    addToken(type, nullptr);
}

/// @brief Add a token to the vector of tokens
/// @param type the type of the token
/// @param literal the literal assosciated with it e.g. string names etc or double values
void Scanner::addToken(TokenType type, std::unique_ptr<Object>  literal){
    std::string text = this->source.substr(start, current-start);
    tokens->push_back(std::make_unique<Token>(type, text, std::move(literal), line));
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
void Scanner::string() {
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
    std::string val = source.substr(start + 1, current - 2 - start);
    addToken(TokenType::STRING, std::make_unique<Object>(&val));
}

/// @brief Returns if the char is a valid digit
/// @param num char to check
/// @return True if it is a digit between 0 and 9
bool Scanner::isDigit(char num){
    return '0' <= num && num <= '9';
}

/// @brief consumes characters up to a non digit except for decimals which it will consume if the one after is a .
void Scanner::number() {
    while(isDigit(peek())) advance();

    //Handling the decimal case
    if(peek() == '.' && isDigit(peekNext())) {
        // consume . and then consume digits
        advance();

        while(isDigit(peek())) advance();
    }

    addToken(NUMBER,
        std::make_unique<Object>(std::stod(source.substr(start, current - start))));

}

/// @brief returns if a the char is alpha numeric
/// @param c 
/// @return 
bool Scanner::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

/// @brief Returns if the char c is alphabetical or an _
/// @param c the char to check
/// @return True if the character is in the alphabet or an underscore
bool Scanner::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c>= 'A' && c <= 'Z') ||
            c == '_';
}

/// @brief Scans in the the maximal identifier starting at thee current char
void Scanner::identifier() {
    while(isAlphaNumeric(peek())) advance();

    //Checking if the word is a reserved keyword
    std::string text = source.substr(start,current - start);
    auto got = keywords.find(text);
    TokenType type = IDENTIFIER;
    if(got != keywords.end()) type = got->second;

    addToken(type);
}

/// @brief Init a keywords the keywords map
void Scanner::initKeywordsMap() {
    keywords = std::unordered_map<std::string, TokenType>();
    keywords.insert({std::string("and"), AND});
    keywords.insert({std::string("class"), CLASS});
    keywords.insert({std::string("else"), ELSE});
    keywords.insert({std::string("false"), FALSE});
    keywords.insert({std::string("for"), FOR});
    keywords.insert({std::string("fun"), FUN});
    keywords.insert({std::string("if"), IF});
    keywords.insert({std::string("nil"), NIL});
    keywords.insert({std::string("or"), OR});
    keywords.insert({std::string("print"), PRINT});
    keywords.insert({std::string("return"), RETURN});
    keywords.insert({std::string("super"), SUPER});
    keywords.insert({std::string("this"), THIS});
    keywords.insert({std::string("true"), TRUE});
    keywords.insert({std::string("var"), VAR}); 
    keywords.insert({std::string("while"), WHILE});
}

/// @brief Removes whitespace
/// @return A vector of strings which are the individual tokens
std::unique_ptr<std::vector<std::unique_ptr<Token>>> Scanner::scanTokens(){
    while(!this->isAtEnd()){
        this->start = current;
        this->scanToken();
    }

    tokens->push_back(std::make_unique<Token>(TokenType::ENDOFFILE, std::string(""),nullptr, line));
    return std::move(tokens);
}

/// @brief Scans the next token from source and adds it to the vector of tokens
void Scanner::scanToken(){
    char c = advance();
    switch (c) {
        default:
            //Parsing identifiers & reserved words
            if(isAlpha(c)){
                identifier();
            }else{
                //Not an identifier so error!
                Lox::error(line, "Unexpected character.");
            }

        break;

        // Single character matches
        case '(': addToken(TokenType::LEFT_PAREN); break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '{': addToken(TokenType::LEFT_BRACE); break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case ',': addToken(TokenType::COMMA); break;
        //case '.': addToken(TokenType::DOT); break;
        case '-': addToken(TokenType::MINUS); break;
        case '+': addToken(TokenType::PLUS); break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case '*': addToken(TokenType::STAR); break; 
        case '.':
            if (isDigit(peek())){
                //If the next digit is a . then we can parse the rest a number
                number();
            }else{
                addToken(TokenType::DOT);
            }
            break;
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
            }else if (match('*')) {
                // multiline comments
                advance(); // want to skip the *
                advanceMultilineComment();
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
        //String literals
        case '"': 
            string(); 
            break;
        //Number literals
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            number();
            break;
    }
}