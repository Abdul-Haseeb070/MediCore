// Class: Doctor  

#include "Doctor.h"
#include "../utils/MyString.h"

Doctor::Doctor() : Person(), fee(0.0f) {
    specialization[0] = '\0';
}

Doctor::Doctor(int did, const char* dname, const char* dspecialization, const char* dcontact, const char* dpassword, float dfee)
    : Person(did, dname, dcontact, dpassword), fee(dfee) {
    myStrcpy(specialization, dspecialization);
}

Doctor::~Doctor() {}

void Doctor::displayInfo() const {
    std::cout << *this << '\n';
}

const char* Doctor::getRole() const {
    return "Doctor";
}

const char* Doctor::getSpecialization() const {
    return specialization;
}

float Doctor::getFee() const {
    return fee;
}

bool Doctor::operator==(const Doctor& other) const {
    return id == other.id;
}

std::ostream& operator<<(std::ostream& os, const Doctor& doctor) {
    os << "Doctor[id=" << doctor.id
       << ", name=" << doctor.name
       << ", specialization=" << doctor.specialization
       << ", contact=" << doctor.contact
       << ", fee=" << doctor.fee << "]";
    return os;
}
