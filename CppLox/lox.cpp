#include "lox.h"
bool Lox::hadError;

/// @brief New Lox intepreter instance where the script is fed in from the command line
Lox::Lox(){
    hadError = false;
    runPrompt();
}

/// @brief Construct a new lox interpreter instance where the script is fed in from a file
/// @param filename 
Lox::Lox(std::string filename){
    hadError = false;
    runScript(filename);
}

/// @brief runs a LOX script
/// @param filename The path to the file but hopefully just the filename
void Lox::runScript(std::string filename){
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    
    run(buffer.str());

    // Indicate an error in the exit code.
    if (hadError) exit(65);
}

/// @brief runs the lox interactive terminal prompt
void Lox::runPrompt(){
    std::printf(">");
    for (std::string line; std::getline(std::cin, line);) {
        if(line.empty()){
            //exit on empty line
            break;
        }
        run(line);
        printf(">");
        hadError = false;
    }
}

/// @brief Runs LOX script from the source
/// @param source a string containing the LOX script to be lexed etc...
void Lox::run(std::string source){
    std::printf("========== START OF SCRIPT==========\n%s\n========== END OF SCRIPT==========\n\n", source.c_str());
    Scanner scanner(source);
    std::unique_ptr<std::vector<std::unique_ptr<Token>>>  tokens = scanner.scanTokens();
    for(auto&& t : *tokens.get()){ //cheating atm and just getting the raw ptr oops
        std::printf("TOKEN: %s\n", t->toString().c_str());
    }

    Parser parser = Parser(std::move(tokens));
    std::unique_ptr<Expr> expression = parser.parse();
    
    ASTPrinter* printer = new ASTPrinter();
    printer->print(*expression.get());
    std::printf("%s",expression.get()->printResult.c_str());
}

/// @brief Report to the command line
/// @param line The line of the error
/// @param where extra details of the error
/// @param message The error message
void Lox::report(int line, char* where, const char* message){
     std::printf("[line %i], Error %s: %s\n",line, where, message);
     hadError = true;
    }

/// @brief Report an error to the console
/// @param line the line the error occured on
/// @param message the error message
void Lox::error(int line, const char* message){
    char temp = 0; report(line,&temp, message);
}
