#pragma once
#include <memory>
#include <vector>
#include "object.h"
#include "interpreter.h"
#include "environment.h"


class LoxFunction : public CallableObject {
private:

public:
    ~LoxFunction() = default;
    LoxFunction(unique_ptr<Token> name, unique_ptr<vector<unique_ptr<Token>>> params, unique_ptr<vector<unique_ptr<Stmt>>> body);
    unique_ptr<Object> call(Interpreter& interpreter, vector<unique_ptr<Object>>& arguments) override;
    int arity() override;
    CallableObject* dup() override;
    unique_ptr<FunctionStmt> declaration;
};