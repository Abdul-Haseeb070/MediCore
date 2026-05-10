// Class: FileHandler  

#include "FileHandler.h"
#include "../exceptions/FileNotFoundException.h"
#include "../utils/MyString.h"
#include <fstream>
#include <ctime>

namespace {
const char* PATIENTS_FILE = "data/patients.txt";
const char* DOCTORS_FILE = "data/doctors.txt";
const char* ADMIN_FILE = "data/admin.txt";
const char* APPOINTMENTS_FILE = "data/appointments.txt";
const char* PRESCRIPTIONS_FILE = "data/prescriptions.txt";
const char* BILLS_FILE = "data/bills.txt";
const char* SECURITY_LOG_FILE = "data/security_log.txt";
const char* DISCHARGED_FILE = "data/discharged.txt";

void freeTokens(char** tokens, int count) {
    for (int i = 0; i < count; ++i) {
        delete[] tokens[i];
    }
}

void writeFloat2(std::ofstream& out, float value) {
    char buf[64];
    myFtoa(value, buf, 2);
    out << buf;
}
}

void FileHandler::loadPatients(Storage<Patient>& patients) {
    std::ifstream in(PATIENTS_FILE);
    if (!in.is_open()) {
        throw FileNotFoundException("Could not open data/patients.txt");
    }

    char line[1024];
    while (in.getline(line, 1024)) {
        if (myStrlen(line) == 0) {
            continue;
        }

        char* tokens[7];
        int count = myTokenize(line, ',', tokens, 7);
        if (count == 7) {
            for (int i = 0; i < count; ++i) {
                myStrTrim(tokens[i]);
            }
            Patient p(myAtoi(tokens[0]), tokens[1], myAtoi(tokens[2]), tokens[3], tokens[4], tokens[5], myAtof(tokens[6]));
            patients.add(p);
        }
        freeTokens(tokens, count);
    }
}

void FileHandler::loadDoctors(Storage<Doctor>& doctors) {
    std::ifstream in(DOCTORS_FILE);
    if (!in.is_open()) {
        throw FileNotFoundException("Could not open data/doctors.txt");
    }

    char line[1024];
    while (in.getline(line, 1024)) {
        if (myStrlen(line) == 0) {
            continue;
        }

        char* tokens[6];
        int count = myTokenize(line, ',', tokens, 6);
        if (count == 6) {
            for (int i = 0; i < count; ++i) {
                myStrTrim(tokens[i]);
            }
            Doctor d(myAtoi(tokens[0]), tokens[1], tokens[2], tokens[3], tokens[4], myAtof(tokens[5]));
            doctors.add(d);
        }
        freeTokens(tokens, count);
    }
}

void FileHandler::loadAdmin(Admin*& admin) {
    std::ifstream in(ADMIN_FILE);
    if (!in.is_open()) {
        throw FileNotFoundException("Could not open data/admin.txt");
    }

    char line[512];
    if (in.getline(line, 512)) {
        char* tokens[3];
        int count = myTokenize(line, ',', tokens, 3);
        if (count == 3) {
            for (int i = 0; i < count; ++i) {
                myStrTrim(tokens[i]);
            }
            admin = new Admin(myAtoi(tokens[0]), tokens[1], tokens[2]);
        }
        freeTokens(tokens, count);
    }
}

void FileHandler::loadAppointments(Storage<Appointment>& appointments) {
    std::ifstream in(APPOINTMENTS_FILE);
    if (!in.is_open()) {
        throw FileNotFoundException("Could not open data/appointments.txt");
    }

    char line[1024];
    while (in.getline(line, 1024)) {
        if (myStrlen(line) == 0) {
            continue;
        }

        char* tokens[6];
        int count = myTokenize(line, ',', tokens, 6);
        if (count == 6) {
            for (int i = 0; i < count; ++i) {
                myStrTrim(tokens[i]);
            }
            Appointment a(myAtoi(tokens[0]), myAtoi(tokens[1]), myAtoi(tokens[2]), tokens[3], tokens[4], tokens[5]);
            appointments.add(a);
        }
        freeTokens(tokens, count);
    }
}

void FileHandler::loadBills(Storage<Bill>& bills) {
    std::ifstream in(BILLS_FILE);
    if (!in.is_open()) {
        throw FileNotFoundException("Could not open data/bills.txt");
    }

    char line[1024];
    while (in.getline(line, 1024)) {
        if (myStrlen(line) == 0) {
            continue;
        }

        char* tokens[6];
        int count = myTokenize(line, ',', tokens, 6);
        if (count == 6) {
            for (int i = 0; i < count; ++i) {
                myStrTrim(tokens[i]);
            }
            Bill b(myAtoi(tokens[0]), myAtoi(tokens[1]), myAtoi(tokens[2]), myAtof(tokens[3]), tokens[4], tokens[5]);
            bills.add(b);
        }
        freeTokens(tokens, count);
    }
}

void FileHandler::loadPrescriptions(Storage<Prescription>& prescriptions) {
    std::ifstream in(PRESCRIPTIONS_FILE);
    if (!in.is_open()) {
        throw FileNotFoundException("Could not open data/prescriptions.txt");
    }

    char line[1400];
    while (in.getline(line, 1400)) {
        if (myStrlen(line) == 0) {
            continue;
        }

        char* tokens[7];
        int count = myTokenize(line, ',', tokens, 7);
        if (count == 7) {
            for (int i = 0; i < count; ++i) {
                myStrTrim(tokens[i]);
            }
            Prescription p(myAtoi(tokens[0]), myAtoi(tokens[1]), myAtoi(tokens[2]), myAtoi(tokens[3]), tokens[4], tokens[5], tokens[6]);
            prescriptions.add(p);
        }
        freeTokens(tokens, count);
    }
}

void FileHandler::saveAllPatients(Storage<Patient>& patients) {
    std::ofstream out(PATIENTS_FILE, std::ios::trunc);
    if (!out.is_open()) {
        throw FileNotFoundException("Could not open data/patients.txt");
    }

    Patient* list = patients.getAll();
    int n = patients.size();
    for (int i = 0; i < n; ++i) {
        out << list[i].getId() << ','
            << list[i].getName() << ','
            << list[i].getAge() << ','
            << list[i].getGender() << ','
            << list[i].getContact() << ','
            << list[i].getPassword() << ',';
        writeFloat2(out, list[i].getBalance());
        out << '\n';
    }
}

void FileHandler::saveAllDoctors(Storage<Doctor>& doctors) {
    std::ofstream out(DOCTORS_FILE, std::ios::trunc);
    if (!out.is_open()) {
        throw FileNotFoundException("Could not open data/doctors.txt");
    }

    Doctor* list = doctors.getAll();
    int n = doctors.size();
    for (int i = 0; i < n; ++i) {
        out << list[i].getId() << ','
            << list[i].getName() << ','
            << list[i].getSpecialization() << ','
            << list[i].getContact() << ','
            << list[i].getPassword() << ',';
        writeFloat2(out, list[i].getFee());
        out << '\n';
    }
}

void FileHandler::saveAllAppointments(Storage<Appointment>& appointments) {
    std::ofstream out(APPOINTMENTS_FILE, std::ios::trunc);
    if (!out.is_open()) {
        throw FileNotFoundException("Could not open data/appointments.txt");
    }

    Appointment* list = appointments.getAll();
    int n = appointments.size();
    for (int i = 0; i < n; ++i) {
        out << list[i].getAppointmentId() << ','
            << list[i].getPatientId() << ','
            << list[i].getDoctorId() << ','
            << list[i].getDate() << ','
            << list[i].getTimeSlot() << ','
            << list[i].getStatus() << '\n';
    }
}

void FileHandler::saveAllBills(Storage<Bill>& bills) {
    std::ofstream out(BILLS_FILE, std::ios::trunc);
    if (!out.is_open()) {
        throw FileNotFoundException("Could not open data/bills.txt");
    }

    Bill* list = bills.getAll();
    int n = bills.size();
    for (int i = 0; i < n; ++i) {
        out << list[i].getBillId() << ','
            << list[i].getPatientId() << ','
            << list[i].getAppointmentId() << ',';
        writeFloat2(out, list[i].getAmount());
        out << ','
            << list[i].getStatus() << ','
            << list[i].getDate() << '\n';
    }
}

void FileHandler::saveAllPrescriptions(Storage<Prescription>& prescriptions) {
    std::ofstream out(PRESCRIPTIONS_FILE, std::ios::trunc);
    if (!out.is_open()) {
        throw FileNotFoundException("Could not open data/prescriptions.txt");
    }

    Prescription* list = prescriptions.getAll();
    int n = prescriptions.size();
    for (int i = 0; i < n; ++i) {
        out << list[i].getPrescriptionId() << ','
            << list[i].getAppointmentId() << ','
            << list[i].getPatientId() << ','
            << list[i].getDoctorId() << ','
            << list[i].getDate() << ','
            << list[i].getMedicines() << ','
            << list[i].getNotes() << '\n';
    }
}

void FileHandler::appendPatient(const Patient& patient) {
    std::ofstream out(PATIENTS_FILE, std::ios::app);
    if (!out.is_open()) {
        throw FileNotFoundException("Could not open data/patients.txt");
    }

    out << patient.getId() << ','
        << patient.getName() << ','
        << patient.getAge() << ','
        << patient.getGender() << ','
        << patient.getContact() << ','
        << patient.getPassword() << ',';
    writeFloat2(out, patient.getBalance());
    out << '\n';
}

void FileHandler::appendDoctor(const Doctor& doctor) {
    std::ofstream out(DOCTORS_FILE, std::ios::app);
    if (!out.is_open()) {
        throw FileNotFoundException("Could not open data/doctors.txt");
    }

    out << doctor.getId() << ','
        << doctor.getName() << ','
        << doctor.getSpecialization() << ','
        << doctor.getContact() << ','
        << doctor.getPassword() << ',';
    writeFloat2(out, doctor.getFee());
    out << '\n';
}

void FileHandler::appendAppointment(const Appointment& appointment) {
    std::ofstream out(APPOINTMENTS_FILE, std::ios::app);
    if (!out.is_open()) {
        throw FileNotFoundException("Could not open data/appointments.txt");
    }

    out << appointment.getAppointmentId() << ','
        << appointment.getPatientId() << ','
        << appointment.getDoctorId() << ','
        << appointment.getDate() << ','
        << appointment.getTimeSlot() << ','
        << appointment.getStatus() << '\n';
}

void FileHandler::appendBill(const Bill& bill) {
    std::ofstream out(BILLS_FILE, std::ios::app);
    if (!out.is_open()) {
        throw FileNotFoundException("Could not open data/bills.txt");
    }

    out << bill.getBillId() << ','
        << bill.getPatientId() << ','
        << bill.getAppointmentId() << ',';
    writeFloat2(out, bill.getAmount());
    out << ','
        << bill.getStatus() << ','
        << bill.getDate() << '\n';
}

void FileHandler::appendPrescription(const Prescription& prescription) {
    std::ofstream out(PRESCRIPTIONS_FILE, std::ios::app);
    if (!out.is_open()) {
        throw FileNotFoundException("Could not open data/prescriptions.txt");
    }

    out << prescription.getPrescriptionId() << ','
        << prescription.getAppointmentId() << ','
        << prescription.getPatientId() << ','
        << prescription.getDoctorId() << ','
        << prescription.getDate() << ','
        << prescription.getMedicines() << ','
        << prescription.getNotes() << '\n';
}

void FileHandler::updatePatientById(const Patient& patient, Storage<Patient>& patients) {
    Patient* list = patients.getAll();
    int n = patients.size();
    for (int i = 0; i < n; ++i) {
        if (list[i].getId() == patient.getId()) {
            list[i] = patient;
            break;
        }
    }
    saveAllPatients(patients);
}

void FileHandler::updateAppointmentById(const Appointment& appointment, Storage<Appointment>& appointments) {
    Appointment* list = appointments.getAll();
    int n = appointments.size();
    for (int i = 0; i < n; ++i) {
        if (list[i].getAppointmentId() == appointment.getAppointmentId()) {
            list[i] = appointment;
            break;
        }
    }
    saveAllAppointments(appointments);
}

void FileHandler::updateBillById(const Bill& bill, Storage<Bill>& bills) {
    Bill* list = bills.getAll();
    int n = bills.size();
    for (int i = 0; i < n; ++i) {
        if (list[i].getBillId() == bill.getBillId()) {
            list[i] = bill;
            break;
        }
    }
    saveAllBills(bills);
}

void FileHandler::logSecurityEvent(const char* role, const char* enteredId, const char* result) {
    std::ofstream out(SECURITY_LOG_FILE, std::ios::app);
    if (!out.is_open()) {
        throw FileNotFoundException("Could not open data/security_log.txt");
    }

    std::time_t now = std::time(0);
    std::tm* local = std::localtime(&now);
    char timestamp[64];
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", local);

    out << timestamp << ',' << role << ',' << enteredId << ',' << result << '\n';
}

void FileHandler::archiveDischargedPatient(const Patient& patient, Storage<Appointment>& appointments, Storage<Bill>& bills, Storage<Prescription>& prescriptions) {
    std::ofstream out(DISCHARGED_FILE, std::ios::app);
    if (!out.is_open()) {
        throw FileNotFoundException("Could not open data/discharged.txt");
    }

    out << patient.getId() << ','
        << patient.getName() << ','
        << patient.getAge() << ','
        << patient.getGender() << ','
        << patient.getContact() << ','
        << patient.getPassword() << ',';
    writeFloat2(out, patient.getBalance());
    out << '\n';

    int pid = patient.getId();

    for (int i = 0; i < appointments.size();) {
        Appointment* allA = appointments.getAll();
        if (allA[i].getPatientId() == pid) {
            appointments.removeById(allA[i].getAppointmentId());
        } else {
            ++i;
        }
    }

    for (int j = 0; j < bills.size();) {
        Bill* allB = bills.getAll();
        if (allB[j].getPatientId() == pid) {
            bills.removeById(allB[j].getBillId());
        } else {
            ++j;
        }
    }

    for (int k = 0; k < prescriptions.size();) {
        Prescription* allP = prescriptions.getAll();
        if (allP[k].getPatientId() == pid) {
            prescriptions.removeById(allP[k].getPrescriptionId());
        } else {
            ++k;
        }
    }

    saveAllAppointments(appointments);
    saveAllBills(bills);
    saveAllPrescriptions(prescriptions);
}
