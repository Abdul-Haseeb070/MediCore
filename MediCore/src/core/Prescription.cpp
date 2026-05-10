// Class: Prescription  

#include "Prescription.h"
#include "../utils/MyString.h"

Prescription::Prescription() : prescriptionId(0), appointmentId(0), patientId(0), doctorId(0) {
    date[0] = '\0';
    medicines[0] = '\0';
    notes[0] = '\0';
}

Prescription::Prescription(int pId, int aId, int patient, int doctor, const char* pDate, const char* meds, const char* pNotes)
    : prescriptionId(pId), appointmentId(aId), patientId(patient), doctorId(doctor) {
    myStrcpy(date, pDate);
    myStrcpy(medicines, meds);
    myStrcpy(notes, pNotes);
}

Prescription::~Prescription() {}

int Prescription::getPrescriptionId() const {
    return prescriptionId;
}

int Prescription::getAppointmentId() const {
    return appointmentId;
}

int Prescription::getPatientId() const {
    return patientId;
}

int Prescription::getDoctorId() const {
    return doctorId;
}

const char* Prescription::getDate() const {
    return date;
}

const char* Prescription::getMedicines() const {
    return medicines;
}

const char* Prescription::getNotes() const {
    return notes;
}

std::ostream& operator<<(std::ostream& os, const Prescription& prescription) {
    os << "Prescription[id=" << prescription.prescriptionId
       << ", appointment=" << prescription.appointmentId
       << ", patient=" << prescription.patientId
       << ", doctor=" << prescription.doctorId
       << ", date=" << prescription.date
       << ", medicines=" << prescription.medicines
       << ", notes=" << prescription.notes << "]";
    return os;
}
