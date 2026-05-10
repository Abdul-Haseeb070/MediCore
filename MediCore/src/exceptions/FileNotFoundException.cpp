// Class: FileNotFoundException  

#include "FileNotFoundException.h"

FileNotFoundException::FileNotFoundException(const char* msg)
    : HospitalException(msg) {}

FileNotFoundException::~FileNotFoundException() {}
