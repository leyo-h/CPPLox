#include <string>
#include <stdio.h>
#include <stdlib.h>

//4 types of literal supported by our object
typedef enum ObjectType_T {
    BOOL_LITERAL,
    NUM_LITERAL,
    STR_LITERAL,
    NIL_LITERAL,
} ObjectType;

typedef union ObjectValue_T {
    bool boolVal;
    double numVal;
    std::string* strPtr;
} ObjectValue;

class Object {
private:
    ObjectType type;
    ObjectValue value;
    std::string strCopy; // We make a copy of the string and then reference that from then on
                        // Not the best for memory ig but easier as we manage creating and deleting the whole object here and dont have to worry about deleting something important...

public:
    Object(ObjectType type, ObjectValue value);
    Object(double value);
    Object(bool value);
    Object(std::string* value);
    Object();
    //Getters as we dont need to change either of the objects properties after we start
    ObjectType getType();
    ObjectValue getValue();

    std::string toString();
};

