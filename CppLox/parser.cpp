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
    if (current - 1 < 0) printf("bad token at the start of the block\n");
    return max(0,current - 1); // clamp the prev to hit -1 should be fine tho as it would be a parser error if we start with an = etc.
}

std::unique_ptr<Expr> Parser::expression(){
    return assignment();
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

    if (match({IDENTIFIER})) {
        return std::make_unique<VariableExpr>(std::move(tokens->at(previous())));
    }
    printf("Uh oh super invalid primary call! \n\n\n");
    return nullptr;

}


std::unique_ptr<Expr> Parser::assignment() {
    //This recursively evaluates r-values so that we can turn them into l-values
    std::unique_ptr<Expr> expr = move(parseOr());

    if (match({EQUAL})) { 
        std::unique_ptr<Token> equals = std::move(tokens->at(previous()));
        std::unique_ptr<Expr> value = assignment();

        if(expr->getType() == VARIABLE) {
            std::unique_ptr<Token> name = move(dynamic_cast<VariableExpr&>(*expr).name); // this should never fail butt just incase :)

            return move(make_unique<AssignExpr>(move(name),move(value)));
        }
        printf("Error invalid assignment target!\n");
        exit(489);
    }
    return move(expr);
}


std::unique_ptr<Expr> Parser::parseOr() {
    // parse or values 
    std::unique_ptr<Expr> expr = parseAnd();
    printf("In PARSE OR\n\n");
    while (match({OR})) { 
        std::unique_ptr<Token> op = std::move(tokens->at(previous()));
        std::unique_ptr<Expr> right = parseAnd();

        expr = make_unique<LogicalExpr>(move(expr), move(op), move(right));
    }
    return move(expr);
}

std::unique_ptr<Expr> Parser::parseAnd() {
    //This recursively evaluates r-values so that we can turn them into l-values
    std::unique_ptr<Expr> expr = move(equality());

    while (match({AND})) { 
        std::unique_ptr<Token> op = std::move(tokens->at(previous()));
        printf("op %s\n",op->getLexme().c_str());
        std::unique_ptr<Expr> right = move(equality());

        expr = make_unique<LogicalExpr>(move(expr), move(op), move(right));
    }
    return move(expr);
}

int Parser::consume(TokenType type, const char* message) {
    if(check(type)) return advance();

    Lox::error(tokens->at(peek())->getLine(), message);
    throw runtime_error("");
}

Parser::Parser(std::unique_ptr<std::vector<unique_ptr<Token>>> setTokens) {
    tokens = std::move(setTokens);
}

//======== Statements=========
unique_ptr<Stmt> Parser::statement() {
    if (match({FOR})) return std::move(forStatement());
    if(match({IF})) return std::move(ifStatement());
    if(match({PRINT})) return std::move(printStatement());
    if(match({WHILE})) return std::move(whileStatement());
    if(match({LEFT_BRACE})) return move(make_unique<BlockStmt>(block()));

    return std::move(expressionStatement());
}

std::unique_ptr<Stmt> Parser::printStatement() {
    unique_ptr<Expr> expr = expression();
    consume(SEMICOLON,"expected semicolon after value");
    return std::move(std::make_unique<PrintStmt>(std::move(expr)));

}

/// @brief Desugaring parser for for statements. Turns the for statement into a block contianing a variable and a while loop
/// @return 
std::unique_ptr<Stmt> Parser::forStatement() {
    consume(LEFT_PAREN,"Expected ( after for ");

    unique_ptr<Stmt> initialiser;
    //Next we parse the initilaizer
    if (match({SEMICOLON})) {
        //If we get a semicolon straight away no initialiser
        initialiser = nullptr;
    } else if (match({VAR})){
        //Declaration
        initialiser = varDeclaration();
    } else{
        initialiser = expressionStatement();
    }

    //Now we get the condition
    unique_ptr<Expr> condition = nullptr;
    if(!check(SEMICOLON)) {
        condition = expression();
    }
    consume(SEMICOLON,"Expected ; after for loop condition");

    //Get incremement
    unique_ptr<Expr> increment = nullptr;
    if(!check(RIGHT_PAREN)) {
        increment = expression();
    }
    consume(RIGHT_PAREN, "Expected ) after for loop increment");

    //Now get the body
    unique_ptr<Stmt> body;
    body = statement();

    unique_ptr<Stmt> output;
    unique_ptr<vector<unique_ptr<Stmt>>> newBlock =  make_unique<vector<unique_ptr<Stmt>>>();
    //Desugaring!!
    if(increment != nullptr) {
        newBlock->push_back(move(body));
        newBlock->push_back(move(make_unique<ExpressionStmt>(move(increment))));
        output = make_unique<BlockStmt>(move(newBlock));

        body = move(output);
    }

    if (condition == nullptr) {
        //condition is null so this is always true
        condition = make_unique<LiteralExpr>(make_unique<Object>(true));
    }
    //Now create the while loop
    unique_ptr<Stmt> whileLoop = make_unique<WhileStmt>(move(condition), move(body));
    //Next we wrap the while loop in a block with the initialiser statement

    unique_ptr<Stmt> forStmt;
     unique_ptr<vector<unique_ptr<Stmt>>> forBlock =  make_unique<vector<unique_ptr<Stmt>>>();
    if (initialiser != nullptr) {
        forBlock->push_back(move(initialiser));
        forBlock->push_back(move(whileLoop));
        forStmt = make_unique<BlockStmt>(move(forBlock));
        return forStmt;
    }
    //no initialiser is just a while loop
    return whileLoop;

}

std::unique_ptr<Stmt> Parser::ifStatement() {
    consume(LEFT_PAREN, "Expected ( after if");
    std::unique_ptr<Expr> condition = expression();
    consume(RIGHT_PAREN, "Expected ) after condition");
    // collect branches
    std::unique_ptr<Stmt> thenBranch = statement();
    std::unique_ptr<Stmt> elseBranch = nullptr;

    if(match({ELSE})) {
        //if the next token is an else token then there is an else branch
        elseBranch = statement();
    }

    return move(make_unique<IfStmt>(move(condition),move(thenBranch),move(elseBranch)));

}

std::unique_ptr<Stmt> Parser::whileStatement() {
    consume(LEFT_PAREN, "Expected ( after if");
    std::unique_ptr<Expr> condition = expression();
    consume(RIGHT_PAREN, "Expected ) after condition");
    std::unique_ptr<Stmt> body = statement();

    return move(make_unique<WhileStmt>(move(condition),move(body)));

}

std::unique_ptr<Stmt> Parser::expressionStatement() {
    unique_ptr<Expr> expr = expression();
    consume(SEMICOLON,"expected semicolon after expression");
    return std::move(std::make_unique<ExpressionStmt>(std::move(expr)));
}


std::unique_ptr<Stmt> Parser::declaration() {
    if(match({VAR})) return move(varDeclaration());
    if(match({LEFT_BRACE})) return move(make_unique<BlockStmt>(block()));
    return std::move(statement());

}

std::unique_ptr<Stmt> Parser::varDeclaration() {
    int tkIdx = consume(IDENTIFIER,"Expected variable name");

    std::unique_ptr<Expr> initialiser = nullptr;

    if (match({EQUAL})) {
        initialiser = expression();
    }

    consume(SEMICOLON,"Expected ; after variable declaration");
    return make_unique<VarStmt>(move(tokens->at(tkIdx)), move(initialiser));
}


std::unique_ptr<vector<unique_ptr<Stmt>>> Parser::block() {
    std::unique_ptr<vector<unique_ptr<Stmt>>> statements = std::make_unique<vector<unique_ptr<Stmt>>>();
    while(!check(RIGHT_BRACE) && !isAtEnd()) {
        statements->push_back(declaration());

    }
    consume(RIGHT_BRACE, "Expected } after block");
    return move(statements);
}


std::unique_ptr<vector<unique_ptr<Stmt>>> Parser::parse(){
    std::unique_ptr<vector<unique_ptr<Stmt>>> statements = make_unique<vector<unique_ptr<Stmt>>>();
    while(!isAtEnd()) {
        statements->push_back(declaration());
    }
    return std::move(statements);
}