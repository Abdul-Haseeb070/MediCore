// Class: AdminMenu  

#include "AdminMenu.h"
#include "../utils/MyString.h"

namespace {
const sf::Color TEXT_PRIMARY(240, 244, 255);
const sf::Color ACCENT(0, 212, 200);
const sf::Color ERROR_RED(255, 80, 80);
const sf::Color SUCCESS_GREEN(80, 220, 130);

int nextDoctorId(Storage<Doctor>& doctors) {
    int maxId = 0;
    Doctor* all = doctors.getAll();
    int n = doctors.size();
    for (int i = 0; i < n; ++i) {
        if (all[i].getId() > maxId) {
            maxId = all[i].getId();
        }
    }
    return maxId + 1;
}
}

AdminMenu::AdminMenu(const sf::Font& font)
    : removeDoctorField(font, 320.0f, 120.0f, 260.0f, 44.0f, 10, false),
      dischargeField(font, 320.0f, 120.0f, 260.0f, 44.0f, 10, false),
      table(320.0f, 120.0f, 930.0f, 540.0f) {
    const char* labels[10] = {
        "Add Doctor", "Remove Doctor", "Patients", "Doctors", "Appointments",
        "Unpaid Bills", "Discharge", "Security Log", "Daily Report", "Logout"
    };

    for (int i = 0; i < 10; ++i) {
        buttons[i] = UIButton(font, labels[i], 20.0f, 22.0f + i * 67.0f, 220.0f, 56.0f, 16u);
    }

    for (int i = 0; i < 5; ++i) {
        addDoctorFields[i] = UITextField(font, 320.0f, 120.0f + i * 56.0f, 420.0f, 44.0f, 70, false);
    }

    feedback[0] = '\0';
}

AdminMenu::~AdminMenu() {}

void AdminMenu::updateHover(sf::Vector2i mousePos) {
    for (int i = 0; i < 10; ++i) {
        buttons[i].updateHover(mousePos);
    }
}

void AdminMenu::handleEvent(const sf::Event& event, GUI& gui) {
    if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2i pos = mouse->position;
        for (int i = 0; i < 10; ++i) {
            if (buttons[i].handleClick(pos)) {
                if (i == 0) gui.setState(AppState::ADMIN_ADD_DOCTOR);
                if (i == 1) gui.setState(AppState::ADMIN_REMOVE_DOCTOR);
                if (i == 2) gui.setState(AppState::ADMIN_ALL_PATIENTS);
                if (i == 3) gui.setState(AppState::ADMIN_ALL_DOCTORS);
                if (i == 4) gui.setState(AppState::ADMIN_ALL_APPOINTMENTS);
                if (i == 5) gui.setState(AppState::ADMIN_UNPAID_BILLS);
                if (i == 6) gui.setState(AppState::ADMIN_DISCHARGE);
                if (i == 7) gui.setState(AppState::ADMIN_SECURITY_LOG);
                if (i == 8) gui.setState(AppState::ADMIN_DAILY_REPORT);
                if (i == 9) gui.logout();
            }
        }

        for (int i = 0; i < 5; ++i) {
            addDoctorFields[i].setFocused(addDoctorFields[i].contains(pos));
        }
        removeDoctorField.setFocused(removeDoctorField.contains(pos));
        dischargeField.setFocused(dischargeField.contains(pos));
    }

    if (const auto* wheel = event.getIf<sf::Event::MouseWheelScrolled>()) {
        table.handleScroll(wheel->delta, 100, 15);
    }

    if (const auto* text = event.getIf<sf::Event::TextEntered>()) {
        if (gui.getState() == AppState::ADMIN_ADD_DOCTOR) {
            for (int i = 0; i < 5; ++i) {
                addDoctorFields[i].handleText(text->unicode);
            }
        }
        if (gui.getState() == AppState::ADMIN_REMOVE_DOCTOR) {
            removeDoctorField.handleText(text->unicode);
        }
        if (gui.getState() == AppState::ADMIN_DISCHARGE) {
            dischargeField.handleText(text->unicode);
        }

        if (text->unicode == 10) {
            try {
                AppState s = gui.getState();
                if (s == AppState::ADMIN_ADD_DOCTOR) {
                    if (!Validator::isValidContact(addDoctorFields[2].getValue()) || !Validator::isValidPassword(addDoctorFields[3].getValue()) || !Validator::isPositiveFloat(addDoctorFields[4].getValue())) {
                        throw InvalidInputException("Invalid doctor input fields.");
                    }
                    int id = nextDoctorId(gui.getDoctors());
                    Doctor d(id, addDoctorFields[0].getValue(), addDoctorFields[1].getValue(), addDoctorFields[2].getValue(), addDoctorFields[3].getValue(), myAtof(addDoctorFields[4].getValue()));
                    gui.getDoctors().add(d);
                    gui.getFileHandler().appendDoctor(d);
                    myStrcpy(feedback, "Doctor added.");
                }

                if (s == AppState::ADMIN_REMOVE_DOCTOR) {
                    int doctorId = myAtoi(removeDoctorField.getValue());
                    Appointment* allA = gui.getAppointments().getAll();
                    int n = gui.getAppointments().size();
                    for (int i = 0; i < n; ++i) {
                        if (allA[i].getDoctorId() == doctorId && myStrcmp(allA[i].getStatus(), "pending") == 0) {
                            throw InvalidInputException("Cannot remove doctor with pending appointments.");
                        }
                    }
                    if (gui.getDoctors().removeById(doctorId)) {
                        gui.getFileHandler().saveAllDoctors(gui.getDoctors());
                        myStrcpy(feedback, "Doctor removed.");
                    } else {
                        myStrcpy(feedback, "Doctor not found.");
                    }
                }

                if (s == AppState::ADMIN_DISCHARGE) {
                    int pid = myAtoi(dischargeField.getValue());
                    Patient* p = gui.getPatients().findById(pid);
                    if (!p) {
                        throw InvalidInputException("Patient not found.");
                    }

                    Bill* allB = gui.getBills().getAll();
                    int bCount = gui.getBills().size();
                    for (int i = 0; i < bCount; ++i) {
                        if (allB[i].getPatientId() == pid && myStrcmp(allB[i].getStatus(), "unpaid") == 0) {
                            throw InvalidInputException("Discharge blocked: unpaid bill exists.");
                        }
                    }

                    Appointment* allA = gui.getAppointments().getAll();
                    int aCount = gui.getAppointments().size();
                    for (int i = 0; i < aCount; ++i) {
                        if (allA[i].getPatientId() == pid && myStrcmp(allA[i].getStatus(), "pending") == 0) {
                            throw InvalidInputException("Discharge blocked: pending appointment exists.");
                        }
                    }

                    Patient copy = *p;
                    gui.getPatients().removeById(pid);
                    gui.getFileHandler().archiveDischargedPatient(copy, gui.getAppointments(), gui.getBills(), gui.getPrescriptions());
                    gui.getFileHandler().saveAllPatients(gui.getPatients());
                    myStrcpy(feedback, "Patient discharged and archived.");
                }
            } catch (const HospitalException& ex) {
                myStrcpy(feedback, ex.what());
            }
        }
    }
}

void AdminMenu::draw(sf::RenderWindow& window, GUI& gui) {
    sf::RectangleShape sidebar({250.0f, 720.0f});
    sidebar.setFillColor(sf::Color(12, 28, 50));
    window.draw(sidebar);

    for (int i = 0; i < 10; ++i) {
        buttons[i].draw(window);
    }

    AppState s = gui.getState();
    const char* headers[1] = {"Admin Data"};
    const int maxRows = 100;
    const char** rows = new const char*[maxRows];
    sf::Color* colors = new sf::Color[maxRows];
    int rowCount = 0;

    if (s == AppState::ADMIN_ADD_DOCTOR) {
        const char* labels[5] = {"Name", "Specialization", "Contact", "Password", "Fee"};
        for (int i = 0; i < 5; ++i) {
            sf::Text l(gui.getFont(), labels[i], 18u);
            l.setFillColor(TEXT_PRIMARY);
            l.setPosition({320.0f, 95.0f + i * 56.0f});
            window.draw(l);
            addDoctorFields[i].draw(window);

            bool ok = true;
            if (i == 2) ok = Validator::isValidContact(addDoctorFields[i].getValue());
            if (i == 3) ok = Validator::isValidPassword(addDoctorFields[i].getValue());
            if (i == 4) ok = Validator::isPositiveFloat(addDoctorFields[i].getValue());
            sf::Text tick(gui.getFont(), ok ? "[OK]" : "[X]", 16u);
            tick.setFillColor(ok ? SUCCESS_GREEN : ERROR_RED);
            tick.setPosition({750.0f, 128.0f + i * 56.0f});
            window.draw(tick);
        }
    }

    if (s == AppState::ADMIN_REMOVE_DOCTOR) {
        sf::Text l(gui.getFont(), "Doctor ID", 18u);
        l.setFillColor(TEXT_PRIMARY);
        l.setPosition({320.0f, 95.0f});
        window.draw(l);
        removeDoctorField.draw(window);
    }

    if (s == AppState::ADMIN_DISCHARGE) {
        sf::Text l(gui.getFont(), "Patient ID", 18u);
        l.setFillColor(TEXT_PRIMARY);
        l.setPosition({320.0f, 95.0f});
        window.draw(l);
        dischargeField.draw(window);
    }

    if (s == AppState::ADMIN_ALL_PATIENTS) {
        Patient* all = gui.getPatients().getAll();
        int n = gui.getPatients().size();
        for (int i = 0; i < n; ++i) {
            char* row = new char[260];
            row[0] = '\0';
            char id[16];
            myItoa(all[i].getId(), id);
            myStrcpy(row, "P");
            myStrcat(row, id);
            myStrcat(row, " | ");
            myStrcat(row, all[i].getName());
            myStrcat(row, " | ");
            myStrcat(row, all[i].getContact());
            rows[rowCount] = row;
            colors[rowCount] = sf::Color(25, 42, 70);
            ++rowCount;
        }
    }

    if (s == AppState::ADMIN_ALL_DOCTORS) {
        Doctor* all = gui.getDoctors().getAll();
        int n = gui.getDoctors().size();
        for (int i = 0; i < n; ++i) {
            char* row = new char[260];
            row[0] = '\0';
            char id[16];
            myItoa(all[i].getId(), id);
            myStrcpy(row, "D");
            myStrcat(row, id);
            myStrcat(row, " | ");
            myStrcat(row, all[i].getName());
            myStrcat(row, " | ");
            myStrcat(row, all[i].getSpecialization());
            rows[rowCount] = row;
            colors[rowCount] = sf::Color(25, 42, 70);
            ++rowCount;
        }
    }

    if (s == AppState::ADMIN_ALL_APPOINTMENTS) {
        Appointment* all = gui.getAppointments().getAll();
        int n = gui.getAppointments().size();
        for (int i = 0; i < n; ++i) {
            char* row = new char[260];
            row[0] = '\0';
            char id[16];
            myItoa(all[i].getAppointmentId(), id);
            myStrcpy(row, "A");
            myStrcat(row, id);
            myStrcat(row, " | P");
            char pid[16];
            myItoa(all[i].getPatientId(), pid);
            myStrcat(row, pid);
            myStrcat(row, " D");
            char did[16];
            myItoa(all[i].getDoctorId(), did);
            myStrcat(row, did);
            myStrcat(row, " | ");
            myStrcat(row, all[i].getDate());
            myStrcat(row, " ");
            myStrcat(row, all[i].getTimeSlot());
            myStrcat(row, " | ");
            myStrcat(row, all[i].getStatus());
            rows[rowCount] = row;
            colors[rowCount] = sf::Color(25, 42, 70);
            ++rowCount;
        }
    }

    if (s == AppState::ADMIN_UNPAID_BILLS) {
        Bill* all = gui.getBills().getAll();
        int n = gui.getBills().size();
        char today[15];
        Validator::getCurrentDate(today);
        for (int i = 0; i < n; ++i) {
            if (myStrcmp(all[i].getStatus(), "unpaid") == 0) {
                char* row = new char[300];
                row[0] = '\0';
                char id[16];
                myItoa(all[i].getBillId(), id);
                myStrcpy(row, "Bill ");
                myStrcat(row, id);
                myStrcat(row, " | P");
                char pid[16];
                myItoa(all[i].getPatientId(), pid);
                myStrcat(row, pid);
                myStrcat(row, " | ");
                myStrcat(row, all[i].getDate());
                int days = Validator::daysBetween(all[i].getDate(), today);
                if (days > 7) {
                    myStrcat(row, " [OVERDUE]");
                    colors[rowCount] = ERROR_RED;
                } else {
                    colors[rowCount] = sf::Color(25, 42, 70);
                }
                rows[rowCount] = row;
                ++rowCount;
            }
        }
    }

    if (s == AppState::ADMIN_DAILY_REPORT) {
        char today[15];
        Validator::getCurrentDate(today);

        int total = 0;
        int completed = 0;
        int pending = 0;
        int noshow = 0;
        Appointment* allA = gui.getAppointments().getAll();
        int an = gui.getAppointments().size();
        for (int i = 0; i < an; ++i) {
            if (myStrcmp(allA[i].getDate(), today) == 0) {
                ++total;
                if (myStrcmp(allA[i].getStatus(), "completed") == 0) ++completed;
                if (myStrcmp(allA[i].getStatus(), "pending") == 0) ++pending;
                if (myStrcmp(allA[i].getStatus(), "noshow") == 0) ++noshow;
            }
        }

        float revenue = 0.0f;
        int unpaidCount = 0;
        Bill* allB = gui.getBills().getAll();
        int bn = gui.getBills().size();
        for (int i = 0; i < bn; ++i) {
            if (myStrcmp(allB[i].getDate(), today) == 0 && myStrcmp(allB[i].getStatus(), "paid") == 0) {
                revenue += allB[i].getAmount();
            }
            if (myStrcmp(allB[i].getStatus(), "unpaid") == 0) {
                ++unpaidCount;
            }
        }

        UICard c1(320.0f, 120.0f, 220.0f, 90.0f);
        UICard c2(560.0f, 120.0f, 220.0f, 90.0f);
        UICard c3(800.0f, 120.0f, 220.0f, 90.0f);
        UICard c4(1040.0f, 120.0f, 210.0f, 90.0f);
        c1.draw(window);
        c2.draw(window);
        c3.draw(window);
        c4.draw(window);

        char s1[60];
        char s2[60];
        char s3[60];
        char s4[60];
        s1[0] = '\0'; s2[0] = '\0'; s3[0] = '\0'; s4[0] = '\0';

        myStrcpy(s1, "Total Today: ");
        char b1[16]; myItoa(total, b1); myStrcat(s1, b1);
        myStrcpy(s2, "Revenue: PKR ");
        char b2[24]; myFtoa(revenue, b2, 2); myStrcat(s2, b2);
        myStrcpy(s3, "Unpaid: ");
        char b3[16]; myItoa(unpaidCount, b3); myStrcat(s3, b3);
        myStrcpy(s4, "No-show: ");
        char b4[16]; myItoa(noshow, b4); myStrcat(s4, b4);

        sf::Text t1(gui.getFont(), s1, 17u); t1.setFillColor(ACCENT); t1.setPosition({332.0f, 156.0f});
        sf::Text t2(gui.getFont(), s2, 17u); t2.setFillColor(ACCENT); t2.setPosition({572.0f, 156.0f});
        sf::Text t3(gui.getFont(), s3, 17u); t3.setFillColor(ACCENT); t3.setPosition({812.0f, 156.0f});
        sf::Text t4(gui.getFont(), s4, 17u); t4.setFillColor(ACCENT); t4.setPosition({1052.0f, 156.0f});
        window.draw(t1); window.draw(t2); window.draw(t3); window.draw(t4);

        Doctor* allD = gui.getDoctors().getAll();
        int dn = gui.getDoctors().size();
        for (int i = 0; i < dn; ++i) {
            int dComp = 0;
            int dPend = 0;
            int dNo = 0;
            for (int j = 0; j < an; ++j) {
                if (allA[j].getDoctorId() == allD[i].getId() && myStrcmp(allA[j].getDate(), today) == 0) {
                    if (myStrcmp(allA[j].getStatus(), "completed") == 0) ++dComp;
                    if (myStrcmp(allA[j].getStatus(), "pending") == 0) ++dPend;
                    if (myStrcmp(allA[j].getStatus(), "noshow") == 0) ++dNo;
                }
            }
            char* row = new char[280];
            row[0] = '\0';
            myStrcpy(row, allD[i].getName());
            myStrcat(row, " | C:");
            char x[10]; myItoa(dComp, x); myStrcat(row, x);
            myStrcat(row, " P:");
            char y[10]; myItoa(dPend, y); myStrcat(row, y);
            myStrcat(row, " N:");
            char z[10]; myItoa(dNo, z); myStrcat(row, z);
            rows[rowCount] = row;
            colors[rowCount] = sf::Color(25, 42, 70);
            ++rowCount;
        }
    }

    if (s == AppState::ADMIN_SECURITY_LOG) {
        char* row = new char[200];
        myStrcpy(row, "Security log entries are persisted in data/security_log.txt");
        rows[rowCount] = row;
        colors[rowCount] = sf::Color(25, 42, 70);
        ++rowCount;
    }

    table.draw(window, gui.getFont(), headers, 1, rows, rowCount, 15, colors);

    for (int i = 0; i < rowCount; ++i) {
        delete[] rows[i];
    }
    delete[] rows;
    delete[] colors;

    if (feedback[0] != '\0') {
        sf::Text msg(gui.getFont(), feedback, 18u);
        msg.setFillColor((myStrContains(feedback, "added") || myStrContains(feedback, "removed") || myStrContains(feedback, "archived")) ? SUCCESS_GREEN : ERROR_RED);
        msg.setPosition({320.0f, 670.0f});
        window.draw(msg);
    }
}
