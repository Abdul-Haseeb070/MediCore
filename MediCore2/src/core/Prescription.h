#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

#include <iostream>

class Prescription {
    int prescriptionId;
    int appointmentId;
    int patientId;
    int doctorId;
    char date[15];
    char medicines[500];
    char notes[300];

public:
    /// @brief Construct a default prescription.
    Prescription();

    /// @brief Construct a prescription.
    /// @param pId Prescription id.
    /// @param aId Appointment id.
    /// @param patient Patient id.
    /// @param doctor Doctor id.
    /// @param pDate Date string.
    /// @param meds Medicine text.
    /// @param pNotes Notes text.
    Prescription(int pId, int aId, int patient, int doctor, const char* pDate, const char* meds, const char* pNotes);

    /// @brief Default destructor.
    ~Prescription();

    /// @brief Get prescription id.
    /// @return Identifier value.
    int getPrescriptionId() const;

    /// @brief Get appointment id.
    /// @return Appointment id.
    int getAppointmentId() const;

    /// @brief Get patient id.
    /// @return Patient id.
    int getPatientId() const;

    /// @brief Get doctor id.
    /// @return Doctor id.
    int getDoctorId() const;

    /// @brief Get date.
    /// @return Date text.
    const char* getDate() const;

    /// @brief Get medicines text.
    /// @return Medicine list.
    const char* getMedicines() const;

    /// @brief Get notes text.
    /// @return Notes text.
    const char* getNotes() const;

    /// @brief Stream output helper.
    /// @param os Output stream.
    /// @param prescription Object to print.
    /// @return Output stream.
    friend std::ostream& operator<<(std::ostream& os, const Prescription& prescription);
};

#endif
