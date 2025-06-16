//Leo H CppLox interpreter :) 06/2025
#include "lox.h"


int main(int argc, char* argv[]){
    if(argc > 2 ){
        std::printf("Usage: CppLox [script]");
        exit(64);
    } else if(argc == 2) {
        //Run script
        std::printf("reading from file \n");
        Lox(std::string(argv[1]));
    }else{
        //Run with prompts
        Lox();
    }
}