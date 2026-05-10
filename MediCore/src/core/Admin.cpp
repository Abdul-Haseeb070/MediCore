// Class: Admin  

#include "Admin.h"
#include <iostream>

Admin::Admin() : Person() {}

Admin::Admin(int aid, const char* aname, const char* apassword)
    : Person(aid, aname, "N/A", apassword) {}

Admin::~Admin() {}

void Admin::displayInfo() const {
    std::cout << "Admin[id=" << id << ", name=" << name << "]\n";
}

const char* Admin::getRole() const {
    return "Admin";
}
