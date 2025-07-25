#include "interpreter.h"

Interpreter::Interpreter() {
    globalEnvironment = make_shared<Environment>();
    this->environment = make_shared<Environment>(globalEnvironment);
    globalEnvironment->define(string("clock"), make_shared<Object>(new ClockFunction()));
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
    //printf("dup check %s , %s\n",node.value->toString().c_str(),node.result->toString().c_str());
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


void Interpreter::visit(VariableExpr& node) {
    shared_ptr<Object> evaluated = environment->get(node.name->getLexme()); //oops shouldve just stuck to unique ptrs lol? - nvm makes sense for functions probably but this is annoying 

    node.result = evaluated->dup();
}

void Interpreter::visit(AssignExpr& node) {
    node.value = move(evaluate(move(node.value)));
    environment->assign(*node.name, make_shared<Object>(node.value->result->getType(),node.value->result->getValue()));
    node.result = node.value->result->dup(); // lets us assign and then use the variable :) e.g. print a = 2 -- prints 2!
}

void Interpreter::visit(LogicalExpr& node) {
    node.left = evaluate(move(node.left));

    if(node.op->getType() == TokenType::OR) {
        if(isTruthy(left)) {
            node.result = node.left->result->dup();
            return;
        }
    } else if(!isTruthy(left)){
            node.result = node.left->result->dup();
            return;
    }

    node.right = evaluate(move(node.right));
    node.result =  node.right->result->dup();
    return;
}


void Interpreter::visit(CallExpr& node) {
    node.callee = evaluate(move(node.callee));
    printf("node callee %i\n", node.callee->getType());
    
    unique_ptr<Object> callee = move(node.callee->result);
    unique_ptr<vector<unique_ptr<Object>>> arguments = make_unique<vector<unique_ptr<Object>>>();

    //Evaluate arguments 
    for(int i = 0; i< node.arguments->size(); i++) {
        (*(node.arguments))[i] = move(evaluate(move(node.arguments->at(i))));
        arguments->push_back(move(node.arguments->at(i)->result));

    }

    if(callee->getType() != CALLABLE) {
        printf("can only call functions and classes\n");
        throw runtime_error("Can only call functions and classes");
    }
    CallableObject* function = callee->getValue().callable;
    if(arguments->size() != function->arity()) {
        printf("[Line %i] Number of arguments mismatch. Expected %i but got %i \n",node.paren->getLine(), arguments->size(), function->arity());
        exit(777);
    }
    node.result =  function->call(*this, *arguments);

}


std::unique_ptr<Expr> Interpreter::evaluate(std::unique_ptr<Expr> expr){
    expr->accept(*this);
    //printf("has anything happened when we accept? %i\n",expr->result->getValue().boolVal);
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
    printf("%s",printStmt.expression->result->toString().c_str());
}

void Interpreter::visit(ExpressionStmt& expressionStmt) {
    expressionStmt.expression = evaluate(std::move(expressionStmt.expression));
}

void  Interpreter::visit(VarStmt& node) {
    shared_ptr<Object> value = make_shared<Object>();
    if(node.initialiser != nullptr) {
        printf("!!! initialiser| \n");
        node.initialiser = evaluate(move(node.initialiser));
        value = move(node.initialiser->result);
    }
    printf("About to make object %i , %i\n ",value->getType(), value->getValue());
    environment->define(node.name->getLexme(), make_shared<Object>(value->getType(), value->getValue()));
}


void  Interpreter::visit(BlockStmt& node) {
   node.statements = executeBlock(move(node.statements), make_shared<Environment>(environment));
}

void Interpreter::visit(IfStmt& node) {
    node.condition =  evaluate(move(node.condition));
    if(isTruthy(*node.condition->result)) {
        node.thenBranch = execute(move(node.thenBranch));
    }else if(node.elseBranch != nullptr) {
        node.elseBranch = execute(move(node.elseBranch));
    }
}

void Interpreter::visit(WhileStmt& node) {
    node.condition = evaluate(move(node.condition));
    while(isTruthy(*node.condition->result)) {
        node.body = execute(move(node.body));
        node.condition = evaluate(move(node.condition));
    }
}

void Interpreter::visit(FunctionStmt& node) {
    printf("in define !%s would be cool if we could define that!\n",node.name->getLexme().c_str());
    //we move node.name as well in this call so we need to copy the node ,name
    string name = node.name->getLexme();
    environment->define(name,
        make_shared<Object>(new LoxFunction(move(node.name),move(node.params),move(node.body)))
    );
}

std::unique_ptr<vector<unique_ptr<Stmt>>> Interpreter::executeBlock(std::unique_ptr<vector<unique_ptr<Stmt>>> statements, shared_ptr<Environment> env) {
    shared_ptr<Environment> prevEnvironment = this->environment;
    this->environment = env; // we swap out our current env to the local one
    for(int i =0; i < statements->size(); i++) {
        (*statements)[i] = execute(move(statements->at(i)));
    }
    this->environment = prevEnvironment; // set the environment back to the original one
    return move(statements);
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

