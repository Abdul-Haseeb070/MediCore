// Class: PatientMenu  

#include "PatientMenu.h"
#include "../utils/MyString.h"

namespace {
const sf::Color BG(10, 22, 40);
const sf::Color CARD(20, 35, 60);
const sf::Color ACCENT(0, 212, 200);
const sf::Color TEXT_PRIMARY(240, 244, 255);
const sf::Color ERROR_RED(255, 80, 80);
const sf::Color SUCCESS_GREEN(80, 220, 130);

void setFeedback(char* dst, const char* text) {
    myStrcpy(dst, text);
}

int nextAppointmentId(Storage<Appointment>& appointments) {
    int maxId = 0;
    Appointment* all = appointments.getAll();
    int n = appointments.size();
    for (int i = 0; i < n; ++i) {
        if (all[i].getAppointmentId() > maxId) {
            maxId = all[i].getAppointmentId();
        }
    }
    return maxId + 1;
}

int nextBillId(Storage<Bill>& bills) {
    int maxId = 0;
    Bill* all = bills.getAll();
    int n = bills.size();
    for (int i = 0; i < n; ++i) {
        if (all[i].getBillId() > maxId) {
            maxId = all[i].getBillId();
        }
    }
    return maxId + 1;
}
}

PatientMenu::PatientMenu(const sf::Font& font)
    : specField(font, 320.0f, 120.0f, 420.0f, 44.0f, 70, false),
      dateField(font, 320.0f, 178.0f, 420.0f, 44.0f, 10, false),
      topupField(font, 320.0f, 120.0f, 260.0f, 44.0f, 12, false),
      table(320.0f, 120.0f, 930.0f, 540.0f),
      topupFailCount(0) {
    const char* labels[8] = {
        "Book", "Cancel", "Appointments", "Records", "Bills", "Pay Bill", "Top Up", "Logout"
    };

    for (int i = 0; i < 8; ++i) {
        buttons[i] = UIButton(font, labels[i], 20.0f, 40.0f + i * 76.0f, 220.0f, 60.0f, 18u);
    }
    feedback[0] = '\0';
}

PatientMenu::~PatientMenu() {}

void PatientMenu::updateHover(sf::Vector2i mousePos) {
    for (int i = 0; i < 8; ++i) {
        buttons[i].updateHover(mousePos);
    }
}

void PatientMenu::handleEvent(const sf::Event& event, GUI& gui) {
    if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2i pos = mouse->position;
        for (int i = 0; i < 8; ++i) {
            if (buttons[i].handleClick(pos)) {
                if (i == 0) gui.setState(AppState::PATIENT_BOOK);
                if (i == 1) gui.setState(AppState::PATIENT_CANCEL);
                if (i == 2) gui.setState(AppState::PATIENT_APPOINTMENTS);
                if (i == 3) gui.setState(AppState::PATIENT_RECORDS);
                if (i == 4) gui.setState(AppState::PATIENT_BILLS);
                if (i == 5) gui.setState(AppState::PATIENT_PAY);
                if (i == 6) gui.setState(AppState::PATIENT_TOPUP);
                if (i == 7) gui.logout();
            }
        }

        specField.setFocused(specField.contains(pos));
        dateField.setFocused(dateField.contains(pos));
        topupField.setFocused(topupField.contains(pos));
    }

    if (const auto* wheel = event.getIf<sf::Event::MouseWheelScrolled>()) {
        table.handleScroll(wheel->delta, 100, 15);
    }

    if (const auto* text = event.getIf<sf::Event::TextEntered>()) {
        if (gui.getState() == AppState::PATIENT_BOOK) {
            specField.handleText(text->unicode);
            dateField.handleText(text->unicode);
        }
        if (gui.getState() == AppState::PATIENT_TOPUP) {
            topupField.handleText(text->unicode);
        }

        if (text->unicode == 10) {
            try {
                Patient* patient = gui.getPatients().findById(gui.getCurrentPatientId());
                if (!patient) {
                    return;
                }

                if (gui.getState() == AppState::PATIENT_BOOK) {
                    if (!Validator::isValidDate(dateField.getValue())) {
                        throw InvalidInputException("Invalid date. Use DD-MM-YYYY.");
                    }

                    char entered[100];
                    char target[100];
                    myToLower(entered, specField.getValue());

                    Doctor* doctors = gui.getDoctors().getAll();
                    int dCount = gui.getDoctors().size();
                    Doctor* chosen = 0;

                    for (int i = 0; i < dCount; ++i) {
                        myToLower(target, doctors[i].getSpecialization());
                        if (myStrcmp(entered, target) == 0) {
                            chosen = &doctors[i];
                            break;
                        }
                    }

                    if (!chosen) {
                        throw InvalidInputException("No doctor found for specialization.");
                    }

                    const char* freeSlot = 0;
                    for (int s = 0; s < 8; ++s) {
                        Appointment probe(0, patient->getId(), chosen->getId(), dateField.getValue(), Validator::SLOTS[s], "pending");
                        bool conflict = false;
                        Appointment* allA = gui.getAppointments().getAll();
                        int aCount = gui.getAppointments().size();
                        for (int j = 0; j < aCount; ++j) {
                            if (probe == allA[j]) {
                                conflict = true;
                                break;
                            }
                        }
                        if (!conflict) {
                            freeSlot = Validator::SLOTS[s];
                            break;
                        }
                    }

                    if (!freeSlot) {
                        throw SlotUnavailableException("No free slot for selected date.");
                    }

                    if (patient->getBalance() < chosen->getFee()) {
                        throw InsufficientFundsException("Insufficient funds to book appointment.");
                    }

                    *patient -= chosen->getFee();
                    int aId = nextAppointmentId(gui.getAppointments());
                    int bId = nextBillId(gui.getBills());
                    Appointment a(aId, patient->getId(), chosen->getId(), dateField.getValue(), freeSlot, "pending");
                    Bill b(bId, patient->getId(), aId, chosen->getFee(), "unpaid", dateField.getValue());

                    gui.getAppointments().add(a);
                    gui.getBills().add(b);

                    gui.getFileHandler().saveAllAppointments(gui.getAppointments());
                    gui.getFileHandler().saveAllBills(gui.getBills());
                    gui.getFileHandler().saveAllPatients(gui.getPatients());

                    setFeedback(feedback, "Appointment booked successfully.");
                }

                if (gui.getState() == AppState::PATIENT_CANCEL) {
                    Appointment* allA = gui.getAppointments().getAll();
                    Bill* allB = gui.getBills().getAll();
                    int aCount = gui.getAppointments().size();
                    int bCount = gui.getBills().size();
                    bool done = false;

                    for (int i = 0; i < aCount; ++i) {
                        if (allA[i].getPatientId() == patient->getId() && myStrcmp(allA[i].getStatus(), "pending") == 0) {
                            allA[i].setStatus("cancelled");
                            for (int j = 0; j < bCount; ++j) {
                                if (allB[j].getAppointmentId() == allA[i].getAppointmentId() && myStrcmp(allB[j].getStatus(), "unpaid") == 0) {
                                    allB[j].setStatus("cancelled");
                                    *patient += allB[j].getAmount();
                                    done = true;
                                    break;
                                }
                            }
                            break;
                        }
                    }

                    if (done) {
                        gui.getFileHandler().saveAllAppointments(gui.getAppointments());
                        gui.getFileHandler().saveAllBills(gui.getBills());
                        gui.getFileHandler().saveAllPatients(gui.getPatients());
                        setFeedback(feedback, "First pending appointment cancelled and refunded.");
                    } else {
                        setFeedback(feedback, "No pending appointment found.");
                    }
                }

                if (gui.getState() == AppState::PATIENT_PAY) {
                    Bill* allB = gui.getBills().getAll();
                    int bCount = gui.getBills().size();
                    bool paid = false;
                    for (int i = 0; i < bCount; ++i) {
                        if (allB[i].getPatientId() == patient->getId() && myStrcmp(allB[i].getStatus(), "unpaid") == 0) {
                            if (patient->getBalance() < allB[i].getAmount()) {
                                throw InsufficientFundsException("Insufficient funds to pay bill.");
                            }
                            *patient -= allB[i].getAmount();
                            allB[i].setStatus("paid");
                            paid = true;
                            break;
                        }
                    }
                    if (paid) {
                        gui.getFileHandler().saveAllBills(gui.getBills());
                        gui.getFileHandler().saveAllPatients(gui.getPatients());
                        setFeedback(feedback, "First unpaid bill paid.");
                    } else {
                        setFeedback(feedback, "No unpaid bill found.");
                    }
                }

                if (gui.getState() == AppState::PATIENT_TOPUP) {
                    if (!Validator::isPositiveFloat(topupField.getValue())) {
                        ++topupFailCount;
                        throw InvalidInputException("Top-up amount must be positive.");
                    }
                    *patient += myAtof(topupField.getValue());
                    gui.getFileHandler().saveAllPatients(gui.getPatients());
                    topupFailCount = 0;
                    setFeedback(feedback, "Balance top-up successful.");
                }
            } catch (const HospitalException& ex) {
                setFeedback(feedback, ex.what());
                if (gui.getState() == AppState::PATIENT_TOPUP && topupFailCount >= 3) {
                    gui.setState(AppState::PATIENT_MAIN);
                    topupFailCount = 0;
                }
            }
        }
    }
}

void PatientMenu::draw(sf::RenderWindow& window, GUI& gui) {
    sf::RectangleShape sidebar({250.0f, 720.0f});
    sidebar.setFillColor(sf::Color(12, 28, 50));
    window.draw(sidebar);

    Patient* patient = gui.getPatients().findById(gui.getCurrentPatientId());
    if (patient) {
        sf::Text name(gui.getFont(), patient->getName(), 22u);
        name.setFillColor(TEXT_PRIMARY);
        name.setPosition({18.0f, 8.0f});

        char bal[64];
        bal[0] = '\0';
        myStrcpy(bal, "Balance: PKR ");
        char b[32];
        myFtoa(patient->getBalance(), b, 2);
        myStrcat(bal, b);

        sf::Text balance(gui.getFont(), bal, 16u);
        balance.setFillColor(ACCENT);
        balance.setPosition({18.0f, 40.0f});

        window.draw(name);
        window.draw(balance);
    }

    for (int i = 0; i < 8; ++i) {
        buttons[i].draw(window);
    }

    AppState s = gui.getState();
    if (s == AppState::PATIENT_BOOK) {
        sf::Text t1(gui.getFont(), "Specialization", 18u);
        t1.setFillColor(TEXT_PRIMARY);
        t1.setPosition({320.0f, 95.0f});
        sf::Text t2(gui.getFont(), "Date (DD-MM-YYYY)", 18u);
        t2.setFillColor(TEXT_PRIMARY);
        t2.setPosition({320.0f, 154.0f});
        sf::Text h(gui.getFont(), "Press Enter to auto-book first available slot", 16u);
        h.setFillColor(ACCENT);
        h.setPosition({320.0f, 236.0f});
        specField.draw(window);
        dateField.draw(window);
        window.draw(t1);
        window.draw(t2);
        window.draw(h);
    }

    if (s == AppState::PATIENT_TOPUP) {
        sf::Text t(gui.getFont(), "Top Up Amount", 18u);
        t.setFillColor(TEXT_PRIMARY);
        t.setPosition({320.0f, 95.0f});
        topupField.draw(window);
        window.draw(t);

        char attempts[30];
        attempts[0] = '\0';
        myStrcpy(attempts, "Attempts: ");
        char c[8];
        myItoa(topupFailCount, c);
        myStrcat(attempts, c);
        myStrcat(attempts, "/3");

        sf::Text attemptText(gui.getFont(), attempts, 16u);
        attemptText.setFillColor(ERROR_RED);
        attemptText.setPosition({320.0f, 175.0f});
        window.draw(attemptText);
    }

    if (s == AppState::PATIENT_APPOINTMENTS || s == AppState::PATIENT_BILLS || s == AppState::PATIENT_RECORDS || s == AppState::PATIENT_PAY || s == AppState::PATIENT_CANCEL) {
        const char* headers[1] = {"Records"};
        const int maxRows = 100;
        const char** rows = new const char*[maxRows];
        sf::Color* colors = new sf::Color[maxRows];
        int rowCount = 0;

        if (s == AppState::PATIENT_APPOINTMENTS || s == AppState::PATIENT_CANCEL) {
            Appointment* all = gui.getAppointments().getAll();
            int n = gui.getAppointments().size();

            Appointment** sorted = new Appointment*[n];
            int m = 0;
            for (int i = 0; i < n; ++i) {
                if (all[i].getPatientId() == gui.getCurrentPatientId()) {
                    sorted[m] = &all[i];
                    ++m;
                }
            }

            for (int i = 1; i < m; ++i) {
                Appointment* key = sorted[i];
                int j = i - 1;
                while (j >= 0 && Validator::isDateBefore(key->getDate(), sorted[j]->getDate())) {
                    sorted[j + 1] = sorted[j];
                    --j;
                }
                sorted[j + 1] = key;
            }

            for (int i = 0; i < m; ++i) {
                char* row = new char[220];
                char id[16];
                myItoa(sorted[i]->getAppointmentId(), id);
                row[0] = '\0';
                myStrcpy(row, "Appt ");
                myStrcat(row, id);
                myStrcat(row, " | ");
                myStrcat(row, sorted[i]->getDate());
                myStrcat(row, " ");
                myStrcat(row, sorted[i]->getTimeSlot());
                myStrcat(row, " | ");
                myStrcat(row, sorted[i]->getStatus());
                rows[rowCount] = row;

                if (myStrcmp(sorted[i]->getStatus(), "completed") == 0) colors[rowCount] = SUCCESS_GREEN;
                else if (myStrcmp(sorted[i]->getStatus(), "cancelled") == 0) colors[rowCount] = sf::Color(120, 120, 120);
                else if (myStrcmp(sorted[i]->getStatus(), "noshow") == 0) colors[rowCount] = ERROR_RED;
                else colors[rowCount] = sf::Color(25, 42, 70);

                ++rowCount;
            }
            delete[] sorted;
        }

        if (s == AppState::PATIENT_BILLS || s == AppState::PATIENT_PAY) {
            Bill* all = gui.getBills().getAll();
            int n = gui.getBills().size();
            float outstanding = 0.0f;
            for (int i = 0; i < n; ++i) {
                if (all[i].getPatientId() == gui.getCurrentPatientId()) {
                    char* row = new char[220];
                    char id[16];
                    myItoa(all[i].getBillId(), id);
                    char amt[32];
                    myFtoa(all[i].getAmount(), amt, 2);
                    row[0] = '\0';
                    myStrcpy(row, "Bill ");
                    myStrcat(row, id);
                    myStrcat(row, " | PKR ");
                    myStrcat(row, amt);
                    myStrcat(row, " | ");
                    myStrcat(row, all[i].getStatus());
                    rows[rowCount] = row;
                    if (myStrcmp(all[i].getStatus(), "unpaid") == 0) {
                        outstanding += all[i].getAmount();
                        colors[rowCount] = ERROR_RED;
                    } else {
                        colors[rowCount] = SUCCESS_GREEN;
                    }
                    ++rowCount;
                }
            }

            if (s == AppState::PATIENT_BILLS) {
                char total[64];
                total[0] = '\0';
                myStrcpy(total, "Total Outstanding: PKR ");
                char o[24];
                myFtoa(outstanding, o, 2);
                myStrcat(total, o);
                sf::Text t(gui.getFont(), total, 20u);
                t.setFillColor(ACCENT);
                t.setPosition({330.0f, 670.0f});
                window.draw(t);
            }
        }

        if (s == AppState::PATIENT_RECORDS) {
            Prescription* all = gui.getPrescriptions().getAll();
            int n = gui.getPrescriptions().size();
            for (int i = 0; i < n; ++i) {
                if (all[i].getPatientId() == gui.getCurrentPatientId()) {
                    char* row = new char[420];
                    row[0] = '\0';
                    myStrcpy(row, all[i].getDate());
                    myStrcat(row, " | meds: ");
                    myStrcat(row, all[i].getMedicines());
                    myStrcat(row, " | notes: ");
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
    }

    if (feedback[0] != '\0') {
        sf::Text msg(gui.getFont(), feedback, 18u);
        msg.setFillColor((myStrContains(feedback, "successful") || myStrContains(feedback, "booked") || myStrContains(feedback, "paid")) ? SUCCESS_GREEN : ERROR_RED);
        msg.setPosition({320.0f, 620.0f});
        window.draw(msg);
    }
}
