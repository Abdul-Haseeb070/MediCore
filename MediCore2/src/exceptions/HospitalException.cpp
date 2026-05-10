// Class: HospitalException  

#include "HospitalException.h"
#include "../utils/MyString.h"

HospitalException::HospitalException(const char* msg) {
    myStrcpy(message, msg);
}

const char* HospitalException::what() const {
    return message;
}

HospitalException::~HospitalException() {}
