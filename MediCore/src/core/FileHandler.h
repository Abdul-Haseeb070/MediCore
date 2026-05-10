#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "Storage.h"
#include "Admin.h"

class FileHandler {
public:
    /// @brief Load patients from file.
    /// @param patients Destination storage.
    /// @return Nothing.
    void loadPatients(Storage<Patient>& patients);

    /// @brief Load doctors from file.
    /// @param doctors Destination storage.
    /// @return Nothing.
    void loadDoctors(Storage<Doctor>& doctors);

    /// @brief Load admin from file.
    /// @param admin Destination pointer allocated with new.
    /// @return Nothing.
    void loadAdmin(Admin*& admin);

    /// @brief Load appointments from file.
    /// @param appointments Destination storage.
    /// @return Nothing.
    void loadAppointments(Storage<Appointment>& appointments);

    /// @brief Load bills from file.
    /// @param bills Destination storage.
    /// @return Nothing.
    void loadBills(Storage<Bill>& bills);

    /// @brief Load prescriptions from file.
    /// @param prescriptions Destination storage.
    /// @return Nothing.
    void loadPrescriptions(Storage<Prescription>& prescriptions);

    /// @brief Save all patients.
    /// @param patients Source storage.
    /// @return Nothing.
    void saveAllPatients(Storage<Patient>& patients);

    /// @brief Save all doctors.
    /// @param doctors Source storage.
    /// @return Nothing.
    void saveAllDoctors(Storage<Doctor>& doctors);

    /// @brief Save all appointments.
    /// @param appointments Source storage.
    /// @return Nothing.
    void saveAllAppointments(Storage<Appointment>& appointments);

    /// @brief Save all bills.
    /// @param bills Source storage.
    /// @return Nothing.
    void saveAllBills(Storage<Bill>& bills);

    /// @brief Save all prescriptions.
    /// @param prescriptions Source storage.
    /// @return Nothing.
    void saveAllPrescriptions(Storage<Prescription>& prescriptions);

    /// @brief Append one patient row.
    /// @param patient Item to append.
    /// @return Nothing.
    void appendPatient(const Patient& patient);

    /// @brief Append one doctor row.
    /// @param doctor Item to append.
    /// @return Nothing.
    void appendDoctor(const Doctor& doctor);

    /// @brief Append one appointment row.
    /// @param appointment Item to append.
    /// @return Nothing.
    void appendAppointment(const Appointment& appointment);

    /// @brief Append one bill row.
    /// @param bill Item to append.
    /// @return Nothing.
    void appendBill(const Bill& bill);

    /// @brief Append one prescription row.
    /// @param prescription Item to append.
    /// @return Nothing.
    void appendPrescription(const Prescription& prescription);

    /// @brief Update patient by id and rewrite file.
    /// @param patient Updated object.
    /// @param patients Storage to update.
    /// @return Nothing.
    void updatePatientById(const Patient& patient, Storage<Patient>& patients);

    /// @brief Update appointment by id and rewrite file.
    /// @param appointment Updated object.
    /// @param appointments Storage to update.
    /// @return Nothing.
    void updateAppointmentById(const Appointment& appointment, Storage<Appointment>& appointments);

    /// @brief Update bill by id and rewrite file.
    /// @param bill Updated object.
    /// @param bills Storage to update.
    /// @return Nothing.
    void updateBillById(const Bill& bill, Storage<Bill>& bills);

    /// @brief Append login security event.
    /// @param role User role text.
    /// @param enteredId Entered id text.
    /// @param result Result text.
    /// @return Nothing.
    void logSecurityEvent(const char* role, const char* enteredId, const char* result);

    /// @brief Archive discharged patient and prune linked records in storages.
    /// @param patient Patient record.
    /// @param appointments Appointment storage.
    /// @param bills Bill storage.
    /// @param prescriptions Prescription storage.
    /// @return Nothing.
    void archiveDischargedPatient(const Patient& patient, Storage<Appointment>& appointments, Storage<Bill>& bills, Storage<Prescription>& prescriptions);
};

#endif
