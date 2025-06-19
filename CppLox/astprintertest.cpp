#include "astprinter.h"

//Testing the AST printer
int main(int argc, char* argv[]){
    std::unique_ptr<Expr> expression = std::make_unique<BinaryExpr>(
        std::make_unique<UnaryExpr>(
            std::make_unique<Token>(TokenType::MINUS,"-",static_cast<Object*>(NULL),1),
            std::make_unique<LiteralExpr>(std::make_unique<Object>(123.00))
    ),std::make_unique<Token>(TokenType::STAR,"*",static_cast<Object*>(NULL),1),std::make_unique<GroupingExpr>(
        std::make_unique<LiteralExpr>(std::make_unique<Object>(45.67)
    )));

    ASTPrinter* printer = new ASTPrinter();
    printer->print(*expression.get());
    std::printf("%s",expression.get()->printResult.c_str());
}