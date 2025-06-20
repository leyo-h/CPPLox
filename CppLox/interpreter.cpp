#include "interpreter.h"

Interpreter::Interpreter() {

}

void Interpreter::visit(BinaryExpr& node) {
    node.right = evaluate(std::move(node.right));
    node.left = evaluate(std::move(node.left));

    switch(node.op->getType()){
        case MINUS:
            checkNumberOperands(*node.op.get(), *node.right->result.get(), *node.left->result.get());
            node.result = std::make_unique<Object> (node.left->result->getValue().numVal - node.right->result->getValue().numVal);
            break;
        case SLASH:
            checkNumberOperands(*node.op.get(), *node.right->result.get(), *node.left->result.get());
            node.result = std::make_unique<Object> (node.left->result->getValue().numVal / node.right->result->getValue().numVal);
            break;
        case STAR:
            checkNumberOperands(*node.op.get(), *node.right->result.get(), *node.left->result.get());
            node.result = std::make_unique<Object> (node.left->result->getValue().numVal * node.right->result->getValue().numVal);
            break;
        case PLUS:
            //Plus is special since we can use it to concatenate strings. might come back to let you add nums to strings etc. js style :) **TODO**
            if(node.right->result->getType() == NUM_LITERAL && node.left->result->getType() == NUM_LITERAL){
                node.result = std::make_unique<Object> (node.left->result->getValue().numVal + node.right->result->getValue().numVal);
            }else if(node.right->result->getType() == STR_LITERAL && node.left->result->getType() == STR_LITERAL){
                std::string outputStr = "";
                outputStr += *node.left->result->getValue().strPtr;
                outputStr += *node.right->result->getValue().strPtr;
                node.result = std::make_unique<Object> (&outputStr);
            }else{
                Lox::error(node.op->getLine(),"Runtime Error! Cannot apply + operator to these types");
                exit(123);
            }
            
            break;

        case GREATER:
            checkNumberOperands(*node.op.get(), *node.right->result.get(), *node.left->result.get());
            node.result = std::make_unique<Object> (node.left->result->getValue().numVal > node.right->result->getValue().numVal);
            break;
        case GREATER_EQUAL:
            checkNumberOperands(*node.op.get(), *node.right->result.get(), *node.left->result.get());
            node.result = std::make_unique<Object> (node.left->result->getValue().numVal >= node.right->result->getValue().numVal);
            break;
        case LESS:
            checkNumberOperands(*node.op.get(), *node.right->result.get(), *node.left->result.get());
            node.result = std::make_unique<Object> (node.left->result->getValue().numVal < node.right->result->getValue().numVal);
            break;
        case LESS_EQUAL:
            checkNumberOperands(*node.op.get(), *node.right->result.get(), *node.left->result.get());
            node.result = std::make_unique<Object> (node.left->result->getValue().numVal <= node.right->result->getValue().numVal);
            break;
        case BANG_EQUAL:
            node.result = std::make_unique<Object> (!isEqual(*node.right->result.get(), *node.left->result.get()));
            break;
        case EQUAL_EQUAL:
            node.result = std::make_unique<Object> (isEqual(*node.right->result.get(), *node.left->result.get()));
            break;
        default:
            printf("Error shouldnt make it here... binaryExpr \n");
            break;
    }   

}

/// @brief Evaluates a literal expression and stores the result in node.result
/// @param node 
void Interpreter::visit(LiteralExpr& node) {
    node.result = node.value->dup();
    printf("dup check %s , %s\n",node.value->toString().c_str(),node.result->toString().c_str());
}

/// @brief Evaluates a unary expression
/// @param node expression to evaluate
void Interpreter::visit(UnaryExpr& node) {
    node.right = evaluate(std::move(node.right));
    switch (node.op->getType())
    {
    case MINUS:
        checkNumberOperand(*node.op.get(), *node.right->result.get());
        node.result = std::make_unique<Object>( - node.right->result->getValue().numVal);
        break;
    case BANG:
        node.result = std::make_unique<Object>(!isTruthy(node.right.get()));
    
    default:
        printf("Error shouldnt make it here... unaryExpr \n");
        break;
    }
}

/// @brief Evaluates a grouping expression
/// @param node expression to evaulate
void Interpreter::visit(GroupingExpr& node) {
    printf("Grouping mode \n");
    node.expr = evaluate(std::move(node.expr));
    node.result = node.expr->result->dup();
}

std::unique_ptr<Expr> Interpreter::evaluate(std::unique_ptr<Expr> expr){
    expr->accept(*this);
    printf("has anything happened when we accept? %i\n",expr->result->getValue().boolVal);
    return std::move(expr);
}

/// @brief Returns if a value is true or not
/// @param obj the object to check
/// @return Returns false if the value is NIL or false. We return true for all other values!
bool Interpreter::isTruthy(const Object& obj){
    switch (obj.getType()){
        case NIL_LITERAL:
            return false;
            break;
        case BOOL_LITERAL:
            return obj.getValue().boolVal;
            break;
        default:
            return true;
            break;
    }
    return true;

}

bool Interpreter::isEqual(const Object& left, const Object& right){
    if(left.getType() != right.getType()) return false; // type mismatch so not equal

    switch(left.getType()){
        //we know they are the same type so we only need to look at one !
        case NIL_LITERAL:
            return true;
            break;
        case BOOL_LITERAL: //think this trick might work :) as bool isnt a pointer as long as bool isnt a NaN ig....
            return left.getValue().boolVal == right.getValue().boolVal;
            break;
        case NUM_LITERAL:
            return left.getValue().numVal == right.getValue().numVal;
            break;
        case STR_LITERAL:
            //Compare the contents of the strings not if they point to the same place :)
            return *left.getValue().strPtr == *right.getValue().strPtr;
            break;
        default:
            return false;
            break;
    }

    return false;
}

void Interpreter::checkNumberOperand(const Token &token, const Object& right) {
    if(right.getType()!=NUM_LITERAL) {
        Lox::error(token.getLine(),"Runtime Error! Cannot apply operator to non number");
        throw runtime_error("");
    }
}

void Interpreter::checkNumberOperands(const Token &token, const Object& left, const Object& right) {
    if(right.getType() != NUM_LITERAL || left.getType() != NUM_LITERAL ) {
        printf("\n Types are %i %i \n",left.getType(),right.getType());
        printf("Values are %s, %s\n",left.toString().c_str(), right.toString().c_str());
        Lox::error(token.getLine(),"Cannot apply operator to non number");
        throw runtime_error("");
    }  
}

//Statements
void Interpreter::visit(PrintStmt& printStmt) {
    printStmt.expression = evaluate(std::move(printStmt.expression));
    printf("PRINT RESULT %s",printStmt.expression->result->toString().c_str());
}

void Interpreter::visit(ExpressionStmt& expressionStmt) {
    expressionStmt.expression = evaluate(std::move(expressionStmt.expression));
}

std::unique_ptr<vector<unique_ptr<Stmt>>> Interpreter::interpret(std::unique_ptr<vector<unique_ptr<Stmt>>> statements) {
    //dont really like throwing exceptionss and catching them,....
    try{
        for(int i = 0; i<statements->size();i++){
            (*statements.get())[i] = execute(std::move(statements->at(i)));
        }
    }catch (runtime_error e){
        printf("runtime error :(");
        was_error = true;
    }
    return std::move(statements);
}

unique_ptr<Stmt> Interpreter::execute(unique_ptr<Stmt> statement){
    statement->accept(*this);
    return move(statement);
}