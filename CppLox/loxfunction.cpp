#include "loxfunction.h"
LoxFunction::LoxFunction(unique_ptr<Token> name, unique_ptr<vector<unique_ptr<Token>>> params, unique_ptr<vector<unique_ptr<Stmt>>> body) {
    declaration = make_unique<FunctionStmt>(move(name), move(params), move(body));
    references = 1;
}


unique_ptr<Object> LoxFunction::call(Interpreter& interpreter, vector<unique_ptr<Object>>& arguments) {
    shared_ptr<Environment> env = make_shared<Environment>(interpreter.globalEnvironment);

    //now set up the environment for the function call
    for (int i=0; i<declaration->params->size(); i++) {
        unique_ptr<Object> t = arguments[i]->dup();
        if(t->getType() == CALLABLE) {
            //Special handling for making a shared ptr to a callable
            env->define(*(*(declaration->params))[i], make_shared<Object>(t->getValue().callable->dup()));
        } else {
            env->define(*(*(declaration->params))[i], make_shared<Object>(t->getType(),t->getValue()));
        }
        
    }
    declaration->body = interpreter.executeBlock(move(declaration->body), env);
    return make_unique<Object>(); // return a NIL object atm
}


int LoxFunction::arity() {
    return declaration->params->size();
}


CallableObject* LoxFunction::dup() {
    this->references += 1;
    return this;
}