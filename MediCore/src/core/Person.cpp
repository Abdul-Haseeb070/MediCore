// Class: Person  

#include "Person.h"
#include "../utils/MyString.h"

Person::Person() : id(0) {
    name[0] = '\0';
    contact[0] = '\0';
    password[0] = '\0';
}

Person::Person(int pid, const char* pname, const char* pcontact, const char* ppassword)
    : id(pid) {
    myStrcpy(name, pname);
    myStrcpy(contact, pcontact);
    myStrcpy(password, ppassword);
}

Person::~Person() {}

int Person::getId() const {
    return id;
}

const char* Person::getName() const {
    return name;
}

const char* Person::getContact() const {
    return contact;
}

const char* Person::getPassword() const {
    return password;
}

bool Person::checkPassword(const char* input) const {
    return myStrcmp(password, input) == 0;
}
