#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include "token.h"
#include "lox.h"

class Scanner{
private:
    std::string source;
    std::vector<Token*> tokens;

    //Scanner positions
    int start;
    int current;
    int line;
    bool isAtEnd();

    // Scan individual token
    void scanToken();

    //Scanner helper methods
    char advance();
    void addToken(TokenType type);
    void addToken(TokenType type, Object* literal);
    bool match(char expected);
    char peek();
    void string();
public:
    Scanner(std::string source);
    std::vector<Token*> scanTokens();
};