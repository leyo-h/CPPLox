#pragma once
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <vector>

using namespace std;
class Expr;
class Interpreter;
class CallableObject;

//4 types of literal supported by our object
typedef enum ObjectType_T {
    BOOL_LITERAL,
    NUM_LITERAL,
    STR_LITERAL,
    NIL_LITERAL,
    CALLABLE,
} ObjectType;

typedef union ObjectValue_T {
    bool boolVal;
    double numVal;
    std::string* strPtr;
    CallableObject* callable; // oops raw pointer probably going to cause some errors? if multiple share the same callable object? so going to try make sure they copy themselves...
} ObjectValue;


class Object {
protected:
    ObjectType type;
    ObjectValue value;
    std::string strCopy; // We make a copy of the string and then reference that from then on
                        // Not the best for memory ig but easier as we manage creating and deleting the whole object here and dont have to worry about deleting something important...
public:
    Object(ObjectType type, ObjectValue value);
    Object(double value);
    Object(bool value);
    Object(std::string* value);
    Object(CallableObject* value);
    Object();
    //Getters as we dont need to change either of the objects properties after we start
    ObjectType getType() const;
    ObjectValue getValue() const;

    std::string toString() const;
    std::unique_ptr<Object> dup();
};

class CallableObject {
private:

public:
    virtual ~CallableObject() = default;
    virtual unique_ptr<Object> call(Interpreter& interpreter, vector<unique_ptr<Object>>& arguments) = 0;
    virtual int arity() = 0;
    virtual CallableObject* dup() = 0; // create object with a callable in it...
};