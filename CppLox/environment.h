#pragma once
#include "object.h"
#include <unordered_map>
#include "token.h"
#include <stdexcept>

using namespace std;
class Environment {
private:
    unordered_map<string, shared_ptr<Object>> values; //using a shared pointer here as i think multiple environments can own an object

public:
    void define(string name, shared_ptr<Object> obj);
    void define(Token& name, shared_ptr<Object> obj);
    void assign(Token& name, shared_ptr<Object> obj);

    void listTokens();
    
    shared_ptr<Object> get(string name);
    shared_ptr<Object> get(Token& name);
    shared_ptr<Environment> enclosing;
    Environment();
    Environment(shared_ptr<Environment> enclosing);
};  