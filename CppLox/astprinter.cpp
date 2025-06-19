#include "astprinter.h"
#include <vector>

/// @brief Sets the print result of the
/// @param expr expression to run on
void ASTPrinter::print(Expr& expr){
    expr.accept(*this);
}

/// @brief Creates the parenthesized tree string (the brackets show how tokens/ expr are grouped together in the tree)
/// @param name Name of the object
/// @param exprs subexpressions which are part of the expr to be parenthesized
/// @return the output string
string ASTPrinter::parenthesize(string name, vector<Expr*> exprs){
    string outputStr = "(" + name;

    for (Expr* expr : exprs){
        outputStr += " ";
        (*expr).accept(*this);
        outputStr += (*expr).printResult;
    }
    outputStr += ")";

    return outputStr;
}

// Expression prints - To return set the printResult variable of the object
void ASTPrinter::visit(BinaryExpr& binaryExpr) {
    binaryExpr.printResult = parenthesize(binaryExpr.op.get()->getLexme(), {binaryExpr.left.get(), binaryExpr.right.get() });
}

void ASTPrinter::visit(LiteralExpr& literalExpr) {
    literalExpr.printResult = literalExpr.value.get()->toString();
}

void ASTPrinter::visit(UnaryExpr& unaryExpr) {
    unaryExpr.printResult = parenthesize(unaryExpr.op.get()->getLexme(), {unaryExpr.right.get()});
}

void ASTPrinter::visit(GroupingExpr& groupingExpr) {
    groupingExpr.printResult = parenthesize("group", {groupingExpr.expr.get()});
}