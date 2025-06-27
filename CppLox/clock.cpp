#include "clock.h"

ClockFunction::ClockFunction() {
    start = time(0);
    references = 1;
}

unique_ptr<Object> ClockFunction::call(Interpreter& interpreter, vector<unique_ptr<Object>>& arguments) {
    return make_unique<Object>(static_cast<double>(time(0)));
}

int ClockFunction::arity() {
    return 0;
}

CallableObject* ClockFunction::dup() {
    return (new ClockFunction());
}