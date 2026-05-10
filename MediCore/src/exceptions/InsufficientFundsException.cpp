// Class: InsufficientFundsException  

#include "InsufficientFundsException.h"

InsufficientFundsException::InsufficientFundsException(const char* msg)
    : HospitalException(msg) {}

InsufficientFundsException::~InsufficientFundsException() {}
