// Class: DoctorMenu  

#include "DoctorMenu.h"
#include "../utils/MyString.h"

namespace {
const sf::Color TEXT_PRIMARY(240, 244, 255);
const sf::Color ACCENT(0, 212, 200);
const sf::Color ERROR_RED(255, 80, 80);
const sf::Color SUCCESS_GREEN(80, 220, 130);
}

DoctorMenu::DoctorMenu(const sf::Font& font)
    : appointmentIdField(font, 320.0f, 120.0f, 250.0f, 44.0f, 10, false),
      medicinesField(font, 320.0f, 180.0f, 880.0f, 44.0f, 120, false),
      notesField(font, 320.0f, 236.0f, 880.0f, 44.0f, 120, false),
      historyPatientField(font, 320.0f, 120.0f, 250.0f, 44.0f, 10, false),
      table(320.0f, 120.0f, 930.0f, 540.0f) {
    const char* labels[6] = {"Today", "Complete", "No-Show", "Prescription", "History", "Logout"};
    for (int i = 0; i < 6; ++i) {
        buttons[i] = UIButton(font, labels[i], 20.0f, 40.0f + i * 88.0f, 220.0f, 66.0f, 18u);
    }
    feedback[0] = '\0';
}

DoctorMenu::~DoctorMenu() {}

void DoctorMenu::updateHover(sf::Vector2i mousePos) {
    for (int i = 0; i < 6; ++i) {
        buttons[i].updateHover(mousePos);
    }
}

void DoctorMenu::handleEvent(const sf::Event& event, GUI& gui) {
    if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2i pos = mouse->position;
        for (int i = 0; i < 6; ++i) {
            if (buttons[i].handleClick(pos)) {
                if (i == 0) gui.setState(AppState::DOCTOR_TODAY);
                if (i == 1) gui.setState(AppState::DOCTOR_COMPLETE);
                if (i == 2) gui.setState(AppState::DOCTOR_NOSHOW);
                if (i == 3) gui.setState(AppState::DOCTOR_PRESCRIPTION);
                if (i == 4) gui.setState(AppState::DOCTOR_HISTORY);
                if (i == 5) gui.logout();
            }
        }
        appointmentIdField.setFocused(appointmentIdField.contains(pos));
        medicinesField.setFocused(medicinesField.contains(pos));
        notesField.setFocused(notesField.contains(pos));
        historyPatientField.setFocused(historyPatientField.contains(pos));
    }

    if (const auto* wheel = event.getIf<sf::Event::MouseWheelScrolled>()) {
        table.handleScroll(wheel->delta, 100, 15);
    }

    if (const auto* text = event.getIf<sf::Event::TextEntered>()) {
        AppState s = gui.getState();
        if (s == AppState::DOCTOR_COMPLETE || s == AppState::DOCTOR_NOSHOW || s == AppState::DOCTOR_PRESCRIPTION) {
            appointmentIdField.handleText(text->unicode);
        }
        if (s == AppState::DOCTOR_PRESCRIPTION) {
            medicinesField.handleText(text->unicode);
            notesField.handleText(text->unicode);
        }
        if (s == AppState::DOCTOR_HISTORY) {
            historyPatientField.handleText(text->unicode);
        }

        if (text->unicode == 10) {
            AppState state = gui.getState();
            try {
                int doctorId = gui.getCurrentDoctorId();
                int apptId = myAtoi(appointmentIdField.getValue());

                if (state == AppState::DOCTOR_COMPLETE || state == AppState::DOCTOR_NOSHOW) {
                    Appointment* appt = gui.getAppointments().findById(apptId);
                    if (!appt || appt->getDoctorId() != doctorId) {
                        throw InvalidInputException("Appointment not found for doctor.");
                    }
                    if (state == AppState::DOCTOR_COMPLETE) {
                        appt->setStatus("completed");
                        myStrcpy(feedback, "Appointment marked completed.");
                    } else {
                        appt->setStatus("noshow");
                        Bill* allB = gui.getBills().getAll();
                        int n = gui.getBills().size();
                        for (int i = 0; i < n; ++i) {
                            if (allB[i].getAppointmentId() == apptId) {
                                allB[i].setStatus("cancelled");
                            }
                        }
                        myStrcpy(feedback, "Appointment marked no-show and bill cancelled.");
                    }
                    gui.getFileHandler().saveAllAppointments(gui.getAppointments());
                    gui.getFileHandler().saveAllBills(gui.getBills());
                }

                if (state == AppState::DOCTOR_PRESCRIPTION) {
                    Appointment* appt = gui.getAppointments().findById(apptId);
                    if (!appt || appt->getDoctorId() != doctorId || myStrcmp(appt->getStatus(), "completed") != 0) {
                        throw InvalidInputException("Only completed own appointments allowed.");
                    }
                    Prescription* all = gui.getPrescriptions().getAll();
                    int pCount = gui.getPrescriptions().size();
                    for (int i = 0; i < pCount; ++i) {
                        if (all[i].getAppointmentId() == apptId) {
                            throw InvalidInputException("Prescription already exists for appointment.");
                        }
                    }

                    char meds[500];
                    char notes[300];
                    int k = 0;
                    while (medicinesField.getValue()[k] != '\0' && k < 499) {
                        meds[k] = medicinesField.getValue()[k];
                        ++k;
                    }
                    meds[k] = '\0';

                    int m = 0;
                    while (notesField.getValue()[m] != '\0' && m < 299) {
                        notes[m] = notesField.getValue()[m];
                        ++m;
                    }
                    notes[m] = '\0';

                    int maxId = 0;
                    for (int i = 0; i < pCount; ++i) {
                        if (all[i].getPrescriptionId() > maxId) {
                            maxId = all[i].getPrescriptionId();
                        }
                    }

                    Prescription p(maxId + 1, apptId, appt->getPatientId(), doctorId, appt->getDate(), meds, notes);
                    gui.getPrescriptions().add(p);
                    gui.getFileHandler().saveAllPrescriptions(gui.getPrescriptions());
                    myStrcpy(feedback, "Prescription saved.");
                }

                if (state == AppState::DOCTOR_HISTORY) {
                    int pid = myAtoi(historyPatientField.getValue());
                    bool allowed = false;
                    Appointment* allA = gui.getAppointments().getAll();
                    int n = gui.getAppointments().size();
                    for (int i = 0; i < n; ++i) {
                        if (allA[i].getPatientId() == pid && allA[i].getDoctorId() == doctorId && myStrcmp(allA[i].getStatus(), "completed") == 0) {
                            allowed = true;
                            break;
                        }
                    }
                    if (!allowed) {
                        throw InvalidInputException("Access denied.");
                    }
                    myStrcpy(feedback, "Access granted. History shown below.");
                }
            } catch (const HospitalException& ex) {
                myStrcpy(feedback, ex.what());
            }
        }
    }
}

void DoctorMenu::draw(sf::RenderWindow& window, GUI& gui) {
    sf::RectangleShape sidebar({250.0f, 720.0f});
    sidebar.setFillColor(sf::Color(12, 28, 50));
    window.draw(sidebar);

    for (int i = 0; i < 6; ++i) {
        buttons[i].draw(window);
    }

    AppState s = gui.getState();
    const char* headers[1] = {"Doctor Data"};
    const int maxRows = 100;
    const char** rows = new const char*[maxRows];
    sf::Color* colors = new sf::Color[maxRows];
    int rowCount = 0;

    if (s == AppState::DOCTOR_COMPLETE || s == AppState::DOCTOR_NOSHOW || s == AppState::DOCTOR_PRESCRIPTION) {
        sf::Text idLabel(gui.getFont(), "Appointment ID", 18u);
        idLabel.setFillColor(TEXT_PRIMARY);
        idLabel.setPosition({320.0f, 95.0f});
        window.draw(idLabel);
        appointmentIdField.draw(window);
    }

    if (s == AppState::DOCTOR_PRESCRIPTION) {
        sf::Text medsLabel(gui.getFont(), "Medicines", 18u);
        medsLabel.setFillColor(TEXT_PRIMARY);
        medsLabel.setPosition({320.0f, 154.0f});
        sf::Text notesLabel(gui.getFont(), "Notes", 18u);
        notesLabel.setFillColor(TEXT_PRIMARY);
        notesLabel.setPosition({320.0f, 210.0f});
        medicinesField.draw(window);
        notesField.draw(window);
        window.draw(medsLabel);
        window.draw(notesLabel);
    }

    if (s == AppState::DOCTOR_HISTORY) {
        sf::Text pLabel(gui.getFont(), "Patient ID", 18u);
        pLabel.setFillColor(TEXT_PRIMARY);
        pLabel.setPosition({320.0f, 95.0f});
        historyPatientField.draw(window);
        window.draw(pLabel);
    }

    if (s == AppState::DOCTOR_TODAY || s == AppState::DOCTOR_COMPLETE || s == AppState::DOCTOR_NOSHOW) {
        char today[15];
        Validator::getCurrentDate(today);
        Appointment* all = gui.getAppointments().getAll();
        int n = gui.getAppointments().size();

        Appointment** filtered = new Appointment*[n];
        int m = 0;
        for (int i = 0; i < n; ++i) {
            if (all[i].getDoctorId() == gui.getCurrentDoctorId() && myStrcmp(all[i].getDate(), today) == 0) {
                filtered[m] = &all[i];
                ++m;
            }
        }

        for (int i = 1; i < m; ++i) {
            Appointment* key = filtered[i];
            int j = i - 1;
            while (j >= 0 && myStrcmp(key->getTimeSlot(), filtered[j]->getTimeSlot()) < 0) {
                filtered[j + 1] = filtered[j];
                --j;
            }
            filtered[j + 1] = key;
        }

        for (int i = 0; i < m; ++i) {
            char* row = new char[220];
            row[0] = '\0';
            char id[16];
            myItoa(filtered[i]->getAppointmentId(), id);
            myStrcpy(row, "Appt ");
            myStrcat(row, id);
            myStrcat(row, " | P");
            char pid[16];
            myItoa(filtered[i]->getPatientId(), pid);
            myStrcat(row, pid);
            myStrcat(row, " | ");
            myStrcat(row, filtered[i]->getTimeSlot());
            myStrcat(row, " | ");
            myStrcat(row, filtered[i]->getStatus());
            rows[rowCount] = row;
            colors[rowCount] = sf::Color(25, 42, 70);
            ++rowCount;
        }

        delete[] filtered;
    }

    if (s == AppState::DOCTOR_HISTORY) {
        int pid = myAtoi(historyPatientField.getValue());
        Prescription* all = gui.getPrescriptions().getAll();
        int n = gui.getPrescriptions().size();
        for (int i = 0; i < n; ++i) {
            if (all[i].getPatientId() == pid && all[i].getDoctorId() == gui.getCurrentDoctorId()) {
                char* row = new char[420];
                row[0] = '\0';
                myStrcpy(row, all[i].getDate());
                myStrcat(row, " | ");
                myStrcat(row, all[i].getMedicines());
                myStrcat(row, " | ");
                myStrcat(row, all[i].getNotes());
                rows[rowCount] = row;
                colors[rowCount] = sf::Color(25, 42, 70);
                ++rowCount;
            }
        }
    }

    table.draw(window, gui.getFont(), headers, 1, rows, rowCount, 15, colors);

    for (int i = 0; i < rowCount; ++i) {
        delete[] rows[i];
    }
    delete[] rows;
    delete[] colors;

    if (feedback[0] != '\0') {
        sf::Text msg(gui.getFont(), feedback, 18u);
        msg.setFillColor((myStrContains(feedback, "saved") || myStrContains(feedback, "granted") || myStrContains(feedback, "marked")) ? SUCCESS_GREEN : ERROR_RED);
        msg.setPosition({320.0f, 670.0f});
        window.draw(msg);
    }
}
