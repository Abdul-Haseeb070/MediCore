// Class: GUI  

#include "GUI.h"
#include "LoginScreen.h"
#include "PatientMenu.h"
#include "DoctorMenu.h"
#include "AdminMenu.h"
#include "../utils/MyString.h"


namespace {
const sf::Color BG(10, 22, 40);
const sf::Color CARD(20, 35, 60);
const sf::Color ACCENT(0, 212, 200);
const sf::Color TEXT_PRIMARY(240, 244, 255);
const sf::Color TEXT_MUTED(140, 160, 190);
const sf::Color ERROR_RED(255, 80, 80);
const sf::Color SUCCESS_GREEN(80, 220, 130);
const sf::Color HOVER(40, 65, 100);
static sf::Font dummyFont;
}

UILabel::UILabel() : text(dummyFont) {}

UILabel::UILabel(const sf::Font& font, const char* value, unsigned int size)
    : text(font, value, size) {
    text.setFillColor(TEXT_PRIMARY);
}


void UILabel::setPosition(float x, float y) {
    text.setPosition({x, y});
}

void UILabel::setString(const char* value) {
    text.setString(value);
}

void UILabel::setColor(const sf::Color& color) {
    text.setFillColor(color);
}

void UILabel::draw(sf::RenderWindow& window) const {
    window.draw(text);
}

UIButton::UIButton() : label(dummyFont), hovered(false), clicked(false) {}

UIButton::UIButton(const sf::Font& font, const char* text, float x, float y, float w, float h, unsigned int size)
    : shape({w, h}), label(font, text, size), hovered(false), clicked(false) {
    shape.setPosition({x, y});
    shape.setFillColor(CARD);
    shape.setOutlineColor(ACCENT);
    shape.setOutlineThickness(2.0f);
    label.setFillColor(TEXT_PRIMARY);
    sf::FloatRect bounds = label.getLocalBounds();
    label.setPosition({x + (w - bounds.size.x) / 2.0f, y + (h - bounds.size.y) / 2.0f - 6.0f});
}

void UIButton::setLabel(const char* text) {
    label.setString(text);
}

void UIButton::setPosition(float x, float y) {
    shape.setPosition({x, y});
    sf::Vector2f size = shape.getSize();
    sf::FloatRect bounds = label.getLocalBounds();
    label.setPosition({x + (size.x - bounds.size.x) / 2.0f, y + (size.y - bounds.size.y) / 2.0f - 6.0f});
}

void UIButton::setSize(float w, float h) {
    shape.setSize({w, h});
}

bool UIButton::contains(sf::Vector2i pos) const {
    return shape.getGlobalBounds().contains(sf::Vector2f(static_cast<float>(pos.x), static_cast<float>(pos.y)));
}

void UIButton::setSelected(bool selected) {
    if (selected) {
        shape.setFillColor(ACCENT);
        label.setFillColor(BG);
    } else {
        shape.setFillColor(CARD);
        label.setFillColor(TEXT_PRIMARY);
    }
}

void UIButton::updateHover(sf::Vector2i pos) {
    hovered = contains(pos);
    if (!clicked) {
        if (hovered) {
            shape.setFillColor(HOVER);
        } else {
            shape.setFillColor(CARD);
        }
    }
}

bool UIButton::handleClick(sf::Vector2i pos) {
    clicked = contains(pos);
    if (clicked) {
        shape.setFillColor(ACCENT);
    }
    return clicked;
}

void UIButton::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(label);
    clicked = false;
}

UITextField::UITextField() : displayText(dummyFont), maxLen(0), focused(false), masked(false) {
    value[0] = '\0';
}

UITextField::UITextField(const sf::Font& font, float x, float y, float w, float h, int maxChars, bool isPassword)
    : box({w, h}), displayText(font, "", 22u), maxLen(maxChars), focused(false), masked(isPassword) {
    box.setPosition({x, y});
    box.setFillColor(CARD);
    box.setOutlineColor(TEXT_MUTED);
    box.setOutlineThickness(2.0f);
    displayText.setPosition({x + 10.0f, y + 8.0f});
    displayText.setFillColor(TEXT_PRIMARY);
    value[0] = '\0';
}

void UITextField::setPosition(float x, float y) {
    box.setPosition({x, y});
    displayText.setPosition({x + 10.0f, y + 8.0f});
}

void UITextField::clear() {
    value[0] = '\0';
    syncDisplay();
}

void UITextField::setValue(const char* text) {
    myStrcpy(value, text);
    syncDisplay();
}

const char* UITextField::getValue() const {
    return value;
}

void UITextField::setFocused(bool isFocused) {
    focused = isFocused;
    box.setOutlineColor(focused ? ACCENT : TEXT_MUTED);
}

bool UITextField::isFocused() const {
    return focused;
}

bool UITextField::contains(sf::Vector2i pos) const {
    return box.getGlobalBounds().contains(sf::Vector2f(static_cast<float>(pos.x), static_cast<float>(pos.y)));
}

void UITextField::handleText(unsigned int unicode) {
    if (!focused) {
        return;
    }

    int len = myStrlen(value);

    if (unicode == 8) {
        if (len > 0) {
            value[len - 1] = '\0';
        }
    } else if (unicode >= 32 && unicode <= 126 && len < maxLen) {
        value[len] = static_cast<char>(unicode);
        value[len + 1] = '\0';
    }

    syncDisplay();
}

void UITextField::syncDisplay() {
    if (!masked) {
        displayText.setString(value);
        return;
    }

    char maskedBuf[256];
    int len = myStrlen(value);
    for (int i = 0; i < len; ++i) {
        maskedBuf[i] = '*';
    }
    maskedBuf[len] = '\0';
    displayText.setString(maskedBuf);
}

void UITextField::draw(sf::RenderWindow& window) {
    window.draw(box);
    window.draw(displayText);
}

UITable::UITable() : scrollOffset(0) {}

UITable::UITable(float x, float y, float w, float h) : container({w, h}), scrollOffset(0) {
    container.setPosition({x, y});
    container.setFillColor(CARD);
    container.setOutlineColor(TEXT_MUTED);
    container.setOutlineThickness(1.0f);
}

void UITable::setRect(float x, float y, float w, float h) {
    container.setPosition({x, y});
    container.setSize({w, h});
}

void UITable::handleScroll(float delta, int rowCount, int visibleRows) {
    if (delta < 0 && scrollOffset < rowCount - visibleRows) {
        ++scrollOffset;
    } else if (delta > 0 && scrollOffset > 0) {
        --scrollOffset;
    }
    if (scrollOffset < 0) {
        scrollOffset = 0;
    }
    if (rowCount <= visibleRows) {
        scrollOffset = 0;
    }
}

int UITable::getScrollOffset() const {
    return scrollOffset;
}

void UITable::draw(sf::RenderWindow& window, const sf::Font& font, const char** headers, int colCount, const char** rows, int rowCount, int visibleRows, const sf::Color* rowColors) {
    window.draw(container);

    sf::Vector2f pos = container.getPosition();
    sf::Vector2f size = container.getSize();
    float rowHeight = 30.0f;
    float colWidth = size.x / static_cast<float>(colCount);

    for (int c = 0; c < colCount; ++c) {
        sf::Text header(font, headers[c], 16u);
        header.setFillColor(ACCENT);
        header.setPosition({pos.x + c * colWidth + 6.0f, pos.y + 4.0f});
        window.draw(header);
    }

    for (int r = 0; r < visibleRows; ++r) {
        int idx = r + scrollOffset;
        if (idx >= rowCount) {
            break;
        }

        sf::RectangleShape tint({size.x, rowHeight});
        tint.setPosition({pos.x, pos.y + 28.0f + r * rowHeight});
        if (rowColors) {
            tint.setFillColor(rowColors[idx]);
        } else {
            tint.setFillColor((idx % 2 == 0) ? sf::Color(25, 42, 70) : sf::Color(18, 32, 56));
        }
        window.draw(tint);

        const char* row = rows[idx];
        sf::Text rowText(font, row, 15u);
        rowText.setFillColor(TEXT_PRIMARY);
        rowText.setPosition({pos.x + 8.0f, pos.y + 32.0f + r * rowHeight});
        window.draw(rowText);
    }
}

UICard::UICard() {}

UICard::UICard(float x, float y, float w, float h) : shape({w, h}) {
    shape.setPosition({x, y});
    shape.setFillColor(CARD);
    shape.setOutlineColor(TEXT_MUTED);
    shape.setOutlineThickness(1.0f);
}

void UICard::setRect(float x, float y, float w, float h) {
    shape.setPosition({x, y});
    shape.setSize({w, h});
}

void UICard::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

GUI::GUI()
    : window(sf::VideoMode({1280u, 720u}), "MediCore"),
      admin(0),
      state(AppState::LOGIN),
      failCount(0),
      accountLocked(false),
      currentPatientId(0),
      currentDoctorId(0),
      loginScreen(0),
      patientMenu(0),
      doctorMenu(0),
      adminMenu(0) {
    if (!font.openFromFile("assets/fonts/Roboto-Regular.ttf")) {
        renderFatalError("Font missing: assets/fonts/Roboto-Regular.ttf");
        window.close();
        return;
    }

    loginScreen = new LoginScreen(font);
    patientMenu = new PatientMenu(font);
    doctorMenu = new DoctorMenu(font);
    adminMenu = new AdminMenu(font);
}

GUI::~GUI() {
    delete loginScreen;
    delete patientMenu;
    delete doctorMenu;
    delete adminMenu;
    delete admin;
}

Storage<Patient>& GUI::getPatients() { return patients; }
Storage<Doctor>& GUI::getDoctors() { return doctors; }
Storage<Appointment>& GUI::getAppointments() { return appointments; }
Storage<Bill>& GUI::getBills() { return bills; }
Storage<Prescription>& GUI::getPrescriptions() { return prescriptions; }
Admin* GUI::getAdmin() const { return admin; }
FileHandler& GUI::getFileHandler() { return fileHandler; }
const sf::Font& GUI::getFont() const { return font; }
int GUI::getCurrentPatientId() const { return currentPatientId; }
int GUI::getCurrentDoctorId() const { return currentDoctorId; }

void GUI::setState(AppState newState) { state = newState; }
AppState GUI::getState() const { return state; }
int GUI::getFailCount() const { return failCount; }
bool GUI::isAccountLocked() const { return accountLocked; }

void GUI::logout() {
    currentPatientId = 0;
    currentDoctorId = 0;
    state = AppState::LOGIN;
    failCount = 0;
    accountLocked = false;
}

void GUI::requestLogin(const char* role, const char* idText, const char* passwordText) {
    processLogin(role, idText, passwordText);
}

void GUI::renderFatalError(const char* message) {
    window.clear(BG);
    sf::RectangleShape panel({700.0f, 160.0f});
    panel.setPosition({290.0f, 260.0f});
    panel.setFillColor(CARD);
    panel.setOutlineColor(ERROR_RED);
    panel.setOutlineThickness(2.0f);

    sf::Text text(font, message, 24u);
    text.setFillColor(ERROR_RED);
    text.setPosition({320.0f, 320.0f});

    window.draw(panel);
    window.draw(text);
    window.display();
}

void GUI::loadData() {
    fileHandler.loadPatients(patients);
    fileHandler.loadDoctors(doctors);
    fileHandler.loadAdmin(admin);
    fileHandler.loadAppointments(appointments);
    fileHandler.loadBills(bills);
    fileHandler.loadPrescriptions(prescriptions);
}

void GUI::processLogin(const char* role, const char* idText, const char* passwordText) {
    if (accountLocked) {
        return;
    }

    int id = myAtoi(idText);
    bool ok = false;

    if (myStrcmp(role, "Patient") == 0) {
        Patient* p = patients.findById(id);
        if (p && p->checkPassword(passwordText)) {
            currentPatientId = id;
            state = AppState::PATIENT_MAIN;
            ok = true;
        }
    } else if (myStrcmp(role, "Doctor") == 0) {
        Doctor* d = doctors.findById(id);
        if (d && d->checkPassword(passwordText)) {
            currentDoctorId = id;
            state = AppState::DOCTOR_MAIN;
            ok = true;
        }
    } else if (myStrcmp(role, "Admin") == 0) {
        if (admin && admin->getId() == id && admin->checkPassword(passwordText)) {
            state = AppState::ADMIN_MAIN;
            ok = true;
        }
    }

    if (ok) {
        failCount = 0;
        return;
    }

    ++failCount;
    if (failCount >= 3) {
        accountLocked = true;
        fileHandler.logSecurityEvent(role, idText, "locked");
    } else {
        fileHandler.logSecurityEvent(role, idText, "failed");
    }
}

int GUI::nextAppointmentId() const {
    int maxId = 0;
    Appointment* all = const_cast<Storage<Appointment>&>(appointments).getAll();
    int n = const_cast<Storage<Appointment>&>(appointments).size();
    for (int i = 0; i < n; ++i) {
        if (all[i].getAppointmentId() > maxId) {
            maxId = all[i].getAppointmentId();
        }
    }
    return maxId + 1;
}

int GUI::nextBillId() const {
    int maxId = 0;
    Bill* all = const_cast<Storage<Bill>&>(bills).getAll();
    int n = const_cast<Storage<Bill>&>(bills).size();
    for (int i = 0; i < n; ++i) {
        if (all[i].getBillId() > maxId) {
            maxId = all[i].getBillId();
        }
    }
    return maxId + 1;
}

int GUI::nextPrescriptionId() const {
    int maxId = 0;
    Prescription* all = const_cast<Storage<Prescription>&>(prescriptions).getAll();
    int n = const_cast<Storage<Prescription>&>(prescriptions).size();
    for (int i = 0; i < n; ++i) {
        if (all[i].getPrescriptionId() > maxId) {
            maxId = all[i].getPrescriptionId();
        }
    }
    return maxId + 1;
}

void GUI::handleEvents() {
    while (const auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        if (state == AppState::LOGIN && loginScreen) {
            loginScreen->updateHover(mousePos);
            loginScreen->handleEvent(*event, *this);
        } else if ((state >= AppState::PATIENT_MAIN && state <= AppState::PATIENT_TOPUP) && patientMenu) {
            patientMenu->updateHover(mousePos);
            patientMenu->handleEvent(*event, *this);
        } else if ((state >= AppState::DOCTOR_MAIN && state <= AppState::DOCTOR_HISTORY) && doctorMenu) {
            doctorMenu->updateHover(mousePos);
            doctorMenu->handleEvent(*event, *this);
        } else if ((state >= AppState::ADMIN_MAIN && state <= AppState::ADMIN_DAILY_REPORT) && adminMenu) {
            adminMenu->updateHover(mousePos);
            adminMenu->handleEvent(*event, *this);
        }
    }
}

void GUI::render() {
    window.clear(BG);

    if (state == AppState::LOGIN && loginScreen) {
        loginScreen->draw(window, font, failCount, accountLocked);
    } else if (state >= AppState::PATIENT_MAIN && state <= AppState::PATIENT_TOPUP && patientMenu) {
        patientMenu->draw(window, *this);
    } else if (state >= AppState::DOCTOR_MAIN && state <= AppState::DOCTOR_HISTORY && doctorMenu) {
        doctorMenu->draw(window, *this);
    } else if (state >= AppState::ADMIN_MAIN && state <= AppState::ADMIN_DAILY_REPORT && adminMenu) {
        adminMenu->draw(window, *this);
    }

    window.display();
}

void GUI::run() {
    try {
        loadData();
    } catch (const HospitalException& ex) {
        renderFatalError(ex.what());
        window.close();
        return;
    }

    while (window.isOpen()) {
        handleEvents();
        render();
        sf::sleep(sf::milliseconds(16));
    }
}
