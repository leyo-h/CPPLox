#include "object.h"

/// @brief Create an object from object type and value
/// @param type the type of the object
/// @param value the value of the object to copy
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
        case BOOL_LITERAL:
            this->type = BOOL_LITERAL;
            this->value.boolVal = value.boolVal;
            break;

    }
}

/// @brief Create a num object
/// @param value the value to store
Object::Object(double value){
    this->type = NUM_LITERAL;
    this->value.numVal = value;
}

/// @brief Create a string object
/// @param value a reference to the string which we make a COPY of 
Object::Object(std::string* value){
    this->strCopy = std::string("");
    this->type = STR_LITERAL;
    this->strCopy += *value;
    this->value.strPtr = &this->strCopy;
}

/// @brief Create a Bool object
/// @param value the assosciated bool value
Object::Object(bool value){
    this->type = BOOL_LITERAL;
    this->value.boolVal = value;
}

/// @brief Create a NIL object
Object::Object(){
    this->type = NIL_LITERAL;
    this->value.numVal = 0;
}

std::string Object::toString() const{
    //printf("do we even have an object?%p, %p\n",type,value);
    switch (type)
    {
    case NUM_LITERAL :
        return std::to_string(value.numVal);
        break;

    case STR_LITERAL :
        if(value.strPtr == NULL) printf("oopsie null string pointer...\n");
        return "" + *value.strPtr;
        break;
    case BOOL_LITERAL:
        if (value.boolVal) return "true";
        return "false";
        break;
    default:
        return std::string("NIL");
    }
}

//Getters as we dont need to change either of the objects properties after we start
ObjectType Object::getType() const{
     return type;
    };

ObjectValue Object::getValue() const {
     return value;
    };

/// @brief Duplicates an object
/// @return returns a pointer to the duplicated object
std::unique_ptr<Object> Object::dup() {
    std::unique_ptr<Object> output = std::make_unique<Object>(type, value);
    return std::move(output);
}