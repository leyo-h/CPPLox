#pragma once
#include "object.h"
#include <time.h>

class ClockFunction : public CallableObject {
private:
    time_t start;
public:
    ~ClockFunction() = default;
    ClockFunction();
    unique_ptr<Object> call(Interpreter& interpreter, vector<unique_ptr<Object>>& arguments) override;
    int arity() override;
    CallableObject* dup() override;
};