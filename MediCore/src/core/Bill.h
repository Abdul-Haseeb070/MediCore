#ifndef BILL_H
#define BILL_H

#include <iostream>

class Bill {
    int billId;
    int patientId;
    int appointmentId;
    float amount;
    char status[20];
    char date[15];

public:
    /// @brief Construct a default bill.
    Bill();

    /// @brief Construct a bill.
    /// @param bId Bill id.
    /// @param pId Patient id.
    /// @param aId Appointment id.
    /// @param amt Amount.
    /// @param bStatus Bill status.
    /// @param bDate Bill date.
    Bill(int bId, int pId, int aId, float amt, const char* bStatus, const char* bDate);

    /// @brief Default destructor.
    ~Bill();

    /// @brief Get bill id.
    /// @return Bill id.
    int getBillId() const;

    /// @brief Get patient id.
    /// @return Patient id.
    int getPatientId() const;

    /// @brief Get appointment id.
    /// @return Appointment id.
    int getAppointmentId() const;

    /// @brief Get amount.
    /// @return Monetary value.
    float getAmount() const;

    /// @brief Get bill status.
    /// @return Status text.
    const char* getStatus() const;

    /// @brief Get bill date.
    /// @return Date text.
    const char* getDate() const;

    /// @brief Update status.
    /// @param value New status.
    /// @return Nothing.
    void setStatus(const char* value);

    /// @brief Stream output helper.
    /// @param os Output stream.
    /// @param bill Object to print.
    /// @return Output stream.
    friend std::ostream& operator<<(std::ostream& os, const Bill& bill);
};

#endif
