#ifndef PATIENT_H
#define PATIENT_H

#include "Person.h"
#include <iostream>

class Patient : public Person {
    int age;
    char gender[5];
    float balance;

public:
    /// @brief Construct a default patient.
    Patient();

    /// @brief Construct a patient instance.
    /// @param pid Patient id.
    /// @param pname Patient name.
    /// @param page Patient age.
    /// @param pgender Patient gender.
    /// @param pcontact Patient contact.
    /// @param ppassword Patient password.
    /// @param pbalance Wallet balance.
    Patient(int pid, const char* pname, int page, const char* pgender, const char* pcontact, const char* ppassword, float pbalance);

    /// @brief Default virtual destructor.
    virtual ~Patient();

    /// @brief Print patient info.
    /// @return Nothing.
    virtual void displayInfo() const;

    /// @brief Role name.
    /// @return Patient role text.
    virtual const char* getRole() const;

    /// @brief Get age.
    /// @return Age.
    int getAge() const;

    /// @brief Get gender.
    /// @return Gender text.
    const char* getGender() const;

    /// @brief Get balance.
    /// @return Current balance.
    float getBalance() const;

    /// @brief Set balance.
    /// @param value New balance.
    /// @return Nothing.
    void setBalance(float value);

    /// @brief Add to patient balance.
    /// @param amount Amount to add.
    /// @return Updated patient reference.
    Patient& operator+=(float amount);

    /// @brief Deduct from patient balance.
    /// @param amount Amount to deduct.
    /// @return Updated patient reference.
    Patient& operator-=(float amount);

    /// @brief Compare patients by id.
    /// @param other Other patient.
    /// @return True when ids are equal.
    bool operator==(const Patient& other) const;

    /// @brief Stream output helper.
    /// @param os Output stream.
    /// @param patient Object to print.
    /// @return Output stream.
    friend std::ostream& operator<<(std::ostream& os, const Patient& patient);
};

#endif
