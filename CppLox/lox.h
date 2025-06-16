#pragma once
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include "scanner.h"

class Lox {
private:
    void runPrompt();
    void runScript(std::string filename);
    void run(std::string source);
    static void report(int line, char* where, const char* message);

    static bool hadError; // has the Lox interpreter encountered an Error
public:
    Lox(); // Run from prompt   
    Lox(std::string filename); // create from script
    static void error(int line, const char* message);
};