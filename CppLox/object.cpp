#include "object.h"

Object::Object(ObjectType type, ObjectValue value) {
    this->strCopy = std::string("");
    switch (type){
        default:
            //Create a NIL object anytime (probably bad practice and should exit out but....)
            this->type = NIL_LITERAL;
            this->value.numVal = 0;
            break;
        case NUM_LITERAL:
            this->type = NUM_LITERAL;
            this->value.numVal = value.numVal;
            break;
        case STR_LITERAL:
            this->type = STR_LITERAL;
            //Copy the string
            this->strCopy = *value.strPtr;
            this->value.strPtr = &this->strCopy; // point to our local copy
            break;

    }
}

Object::Object(double value){
    this->type = NUM_LITERAL;
    this->value.numVal = value;
}

Object::Object(std::string* value){
    this->strCopy = std::string("");
    this->type = STR_LITERAL;
    this->strCopy += *value;
    this->value.strPtr = &this->strCopy;
}

Object::Object(bool value){
    this->type = BOOL_LITERAL;
    this->value.boolVal = value;
}

Object::Object(){
    this->type = NIL_LITERAL;
    this->value.numVal = 0;
}

std::string Object::toString() {
    switch (type)
    {
    case NUM_LITERAL :
        return std::to_string(value.numVal);
        break;

    case STR_LITERAL :
        return *value.strPtr;
        break;
    default:
        return std::string("NIL");
    }
}

//Getters as we dont need to change either of the objects properties after we start
ObjectType Object::getType() {
     return type;
    };

ObjectValue Object::getValue() {
     return value;
    };