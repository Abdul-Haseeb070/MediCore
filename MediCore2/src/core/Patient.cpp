// Class: Patient  

#include "Patient.h"
#include "../utils/MyString.h"

Patient::Patient() : Person(), age(0), balance(0.0f) {
    gender[0] = '\0';
}

Patient::Patient(int pid, const char* pname, int page, const char* pgender, const char* pcontact, const char* ppassword, float pbalance)
    : Person(pid, pname, pcontact, ppassword), age(page), balance(pbalance) {
    myStrcpy(gender, pgender);
}

Patient::~Patient() {}

void Patient::displayInfo() const {
    std::cout << *this << '\n';
}

const char* Patient::getRole() const {
    return "Patient";
}

int Patient::getAge() const {
    return age;
}

const char* Patient::getGender() const {
    return gender;
}

float Patient::getBalance() const {
    return balance;
}

void Patient::setBalance(float value) {
    balance = value;
}

Patient& Patient::operator+=(float amount) {
    balance += amount;
    return *this;
}

Patient& Patient::operator-=(float amount) {
    balance -= amount;
    return *this;
}

bool Patient::operator==(const Patient& other) const {
    return id == other.id;
}

std::ostream& operator<<(std::ostream& os, const Patient& patient) {
    os << "Patient[id=" << patient.id
       << ", name=" << patient.name
       << ", age=" << patient.age
       << ", gender=" << patient.gender
       << ", contact=" << patient.contact
       << ", balance=" << patient.balance << "]";
    return os;
}
