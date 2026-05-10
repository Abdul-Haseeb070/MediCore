// Class: Appointment  

#include "Appointment.h"
#include "../utils/MyString.h"

Appointment::Appointment() : appointmentId(0), patientId(0), doctorId(0) {
    date[0] = '\0';
    timeSlot[0] = '\0';
    status[0] = '\0';
}

Appointment::Appointment(int aId, int pId, int dId, const char* aDate, const char* slot, const char* aStatus)
    : appointmentId(aId), patientId(pId), doctorId(dId) {
    myStrcpy(date, aDate);
    myStrcpy(timeSlot, slot);
    myStrcpy(status, aStatus);
}

Appointment::~Appointment() {}

int Appointment::getAppointmentId() const {
    return appointmentId;
}

int Appointment::getPatientId() const {
    return patientId;
}

int Appointment::getDoctorId() const {
    return doctorId;
}

const char* Appointment::getDate() const {
    return date;
}

const char* Appointment::getTimeSlot() const {
    return timeSlot;
}

const char* Appointment::getStatus() const {
    return status;
}

void Appointment::setStatus(const char* value) {
    myStrcpy(status, value);
}

bool Appointment::operator==(const Appointment& other) const {
    if (myStrcmp(status, "cancelled") == 0 || myStrcmp(other.status, "cancelled") == 0) {
        return false;
    }
    return doctorId == other.doctorId && myStrcmp(date, other.date) == 0 && myStrcmp(timeSlot, other.timeSlot) == 0;
}

std::ostream& operator<<(std::ostream& os, const Appointment& appointment) {
    os << "Appointment[id=" << appointment.appointmentId
       << ", patient=" << appointment.patientId
       << ", doctor=" << appointment.doctorId
       << ", date=" << appointment.date
       << ", slot=" << appointment.timeSlot
       << ", status=" << appointment.status << "]";
    return os;
}
