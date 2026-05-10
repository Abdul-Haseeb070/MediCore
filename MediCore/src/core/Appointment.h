#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <iostream>

class Appointment {
    int appointmentId;
    int patientId;
    int doctorId;
    char date[15];
    char timeSlot[10];
    char status[20];

public:
    /// @brief Construct a default appointment.
    Appointment();

    /// @brief Construct an appointment.
    /// @param aId Appointment id.
    /// @param pId Patient id.
    /// @param dId Doctor id.
    /// @param aDate Date DD-MM-YYYY.
    /// @param slot Time slot HH:MM.
    /// @param aStatus Appointment state.
    Appointment(int aId, int pId, int dId, const char* aDate, const char* slot, const char* aStatus);

    /// @brief Default destructor.
    ~Appointment();

    /// @brief Get appointment id.
    /// @return Appointment id.
    int getAppointmentId() const;

    /// @brief Get patient id.
    /// @return Patient id.
    int getPatientId() const;

    /// @brief Get doctor id.
    /// @return Doctor id.
    int getDoctorId() const;

    /// @brief Get appointment date.
    /// @return Date text.
    const char* getDate() const;

    /// @brief Get slot text.
    /// @return Slot text.
    const char* getTimeSlot() const;

    /// @brief Get status.
    /// @return Status text.
    const char* getStatus() const;

    /// @brief Update status.
    /// @param value New status.
    /// @return Nothing.
    void setStatus(const char* value);

    /// @brief Conflict checker against another appointment.
    /// @param other Other appointment.
    /// @return True when same doctor/date/slot and neither cancelled.
    bool operator==(const Appointment& other) const;

    /// @brief Stream output helper.
    /// @param os Output stream.
    /// @param appointment Object to print.
    /// @return Output stream.
    friend std::ostream& operator<<(std::ostream& os, const Appointment& appointment);
};

#endif
