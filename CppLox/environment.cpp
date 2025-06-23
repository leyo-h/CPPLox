#include "environment.h"

Environment::Environment() {
    values = unordered_map<string, shared_ptr<Object>>();
    enclosing = nullptr;
}

Environment::Environment(shared_ptr<Environment> setEnclosing) {
    //creates new environment nested within enclosing environment
    enclosing = setEnclosing;
    values = unordered_map<string, shared_ptr<Object>>();

}

void Environment::define(string name, shared_ptr<Object> obj) {
    //TODO might have to implement some cleanup here if an object was being used and is assigned over..
    values[name] = std::make_shared<Object>(*obj);
}

shared_ptr<Object> Environment::get(string name) {
    if(values.find(name) == values.end()) {
        //Cannot find the variable so we check the enclosing environment and throw error if its not found!
        if (enclosing != nullptr) return enclosing->get(name);
        printf("ERROR Could not find key w/ name %s\n",name.c_str());
        throw runtime_error("Cannot find jey name");
    }
    return std::make_shared<Object>(*values[name]);   
}

shared_ptr<Object> Environment::get(Token& name) {
    if(values.find(name.getLexme()) == values.end()) {
        //Cannot find the variable so we check the enclosing environment and throw error if its not found!
        if (enclosing != nullptr) return enclosing->get(name);
        printf("ERROR Could not find key w/ name %s on line %i\n",name.getLexme().c_str(),name.getLine());
        throw runtime_error("Cannot find the name");
    }
    return std::make_shared<Object>(*values[name.getLexme()]);   
}



void Environment::define(Token& name, shared_ptr<Object> obj) {
    values[name.getLexme()] = std::make_shared<Object>(*obj);
}

void Environment::assign(Token& name, shared_ptr<Object> obj) {
    if(values.find(name.getLexme()) == values.end()) {
        values[name.getLexme()] = make_shared<Object>(*obj);
        return;
    }
    if (enclosing != nullptr){
        //try assign in the parent env!
        enclosing->assign(name,obj);
        return;
    }
    printf("Error undefined variable %s on line %i",name.getLexme(),name.getLine());
}