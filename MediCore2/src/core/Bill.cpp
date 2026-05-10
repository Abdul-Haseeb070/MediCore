// Class: Bill  

#include "Bill.h"
#include "../utils/MyString.h"

Bill::Bill() : billId(0), patientId(0), appointmentId(0), amount(0.0f) {
    status[0] = '\0';
    date[0] = '\0';
}

Bill::Bill(int bId, int pId, int aId, float amt, const char* bStatus, const char* bDate)
    : billId(bId), patientId(pId), appointmentId(aId), amount(amt) {
    myStrcpy(status, bStatus);
    myStrcpy(date, bDate);
}

Bill::~Bill() {}

int Bill::getBillId() const {
    return billId;
}

int Bill::getPatientId() const {
    return patientId;
}

int Bill::getAppointmentId() const {
    return appointmentId;
}

float Bill::getAmount() const {
    return amount;
}

const char* Bill::getStatus() const {
    return status;
}

const char* Bill::getDate() const {
    return date;
}

void Bill::setStatus(const char* value) {
    myStrcpy(status, value);
}

std::ostream& operator<<(std::ostream& os, const Bill& bill) {
    os << "Bill[id=" << bill.billId
       << ", patient=" << bill.patientId
       << ", appointment=" << bill.appointmentId
       << ", amount=" << bill.amount
       << ", status=" << bill.status
       << ", date=" << bill.date << "]";
    return os;
}
