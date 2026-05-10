#ifndef DOCTOR_H
#define DOCTOR_H

#include "Person.h"
#include <iostream>

class Doctor : public Person {
    char specialization[100];
    float fee;

public:
    /// @brief Construct a default doctor.
    Doctor();

    /// @brief Construct a doctor.
    /// @param did Doctor id.
    /// @param dname Doctor name.
    /// @param dspecialization Doctor specialization.
    /// @param dcontact Doctor contact.
    /// @param dpassword Doctor password.
    /// @param dfee Consultation fee.
    Doctor(int did, const char* dname, const char* dspecialization, const char* dcontact, const char* dpassword, float dfee);

    /// @brief Default virtual destructor.
    virtual ~Doctor();

    /// @brief Print doctor info.
    /// @return Nothing.
    virtual void displayInfo() const;

    /// @brief Role label.
    /// @return Doctor role text.
    virtual const char* getRole() const;

    /// @brief Get specialization.
    /// @return Specialization text.
    const char* getSpecialization() const;

    /// @brief Get consultation fee.
    /// @return Fee amount.
    float getFee() const;

    /// @brief Compare doctors by id.
    /// @param other Other doctor.
    /// @return True when ids are equal.
    bool operator==(const Doctor& other) const;

    /// @brief Stream output helper.
    /// @param os Output stream.
    /// @param doctor Object to print.
    /// @return Output stream.
    friend std::ostream& operator<<(std::ostream& os, const Doctor& doctor);
};

#endif
