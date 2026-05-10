#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>
#include "../core/FileHandler.h"
#include "../core/Validator.h"
#include "../exceptions/HospitalException.h"
#include "../exceptions/FileNotFoundException.h"
#include "../exceptions/InsufficientFundsException.h"
#include "../exceptions/InvalidInputException.h"
#include "../exceptions/SlotUnavailableException.h"

class LoginScreen;
class PatientMenu;
class DoctorMenu;
class AdminMenu;

enum class AppState {
    LOGIN,
    PATIENT_MAIN, PATIENT_BOOK, PATIENT_CANCEL,
    PATIENT_APPOINTMENTS, PATIENT_RECORDS, PATIENT_BILLS,
    PATIENT_PAY, PATIENT_TOPUP,
    DOCTOR_MAIN, DOCTOR_TODAY, DOCTOR_COMPLETE,
    DOCTOR_NOSHOW, DOCTOR_PRESCRIPTION, DOCTOR_HISTORY,
    ADMIN_MAIN, ADMIN_ADD_DOCTOR, ADMIN_REMOVE_DOCTOR,
    ADMIN_ALL_PATIENTS, ADMIN_ALL_DOCTORS, ADMIN_ALL_APPOINTMENTS,
    ADMIN_UNPAID_BILLS, ADMIN_DISCHARGE, ADMIN_SECURITY_LOG,
    ADMIN_DAILY_REPORT
};

class UILabel {
    sf::Text text;

public:
    UILabel();
    UILabel(const sf::Font& font, const char* value, unsigned int size);
    void setPosition(float x, float y);
    void setString(const char* value);
    void setColor(const sf::Color& color);
    void draw(sf::RenderWindow& window) const;
};

class UIButton {
    sf::RectangleShape shape;
    sf::Text label;
    bool hovered;
    bool clicked;

public:
    UIButton();
    UIButton(const sf::Font& font, const char* text, float x, float y, float w, float h, unsigned int size);
    void setLabel(const char* text);
    void setPosition(float x, float y);
    void setSize(float w, float h);
    bool contains(sf::Vector2i pos) const;
    void setSelected(bool selected);
    void updateHover(sf::Vector2i pos);
    bool handleClick(sf::Vector2i pos);
    void draw(sf::RenderWindow& window);
};

class UITextField {
    sf::RectangleShape box;
    sf::Text displayText;
    char value[256];
    int maxLen;
    bool focused;
    bool masked;

public:
    UITextField();
    UITextField(const sf::Font& font, float x, float y, float w, float h, int maxChars, bool isPassword);
    void setPosition(float x, float y);
    void clear();
    void setValue(const char* text);
    const char* getValue() const;
    void setFocused(bool isFocused);
    bool isFocused() const;
    bool contains(sf::Vector2i pos) const;
    void handleText(unsigned int unicode);
    void draw(sf::RenderWindow& window);

private:
    void syncDisplay();
};

class UITable {
    sf::RectangleShape container;
    int scrollOffset;

public:
    UITable();
    UITable(float x, float y, float w, float h);
    void setRect(float x, float y, float w, float h);
    void handleScroll(float delta, int rowCount, int visibleRows);
    int getScrollOffset() const;
    void draw(sf::RenderWindow& window, const sf::Font& font, const char** headers, int colCount, const char** rows, int rowCount, int visibleRows, const sf::Color* rowColors);
};

class UICard {
    sf::RectangleShape shape;

public:
    UICard();
    UICard(float x, float y, float w, float h);
    void setRect(float x, float y, float w, float h);
    void draw(sf::RenderWindow& window) const;
};

class GUI {
    sf::RenderWindow window;
    sf::Font font;

    Storage<Patient> patients;
    Storage<Doctor> doctors;
    Storage<Appointment> appointments;
    Storage<Bill> bills;
    Storage<Prescription> prescriptions;
    Admin* admin;

    FileHandler fileHandler;

    AppState state;
    int failCount;
    bool accountLocked;
    int currentPatientId;
    int currentDoctorId;

    LoginScreen* loginScreen;
    PatientMenu* patientMenu;
    DoctorMenu* doctorMenu;
    AdminMenu* adminMenu;

public:
    GUI();
    ~GUI();

    void run();

    Storage<Patient>& getPatients();
    Storage<Doctor>& getDoctors();
    Storage<Appointment>& getAppointments();
    Storage<Bill>& getBills();
    Storage<Prescription>& getPrescriptions();
    Admin* getAdmin() const;
    FileHandler& getFileHandler();
    const sf::Font& getFont() const;
    int getCurrentPatientId() const;
    int getCurrentDoctorId() const;

    void setState(AppState newState);
    AppState getState() const;
    void logout();
    void requestLogin(const char* role, const char* idText, const char* passwordText);
    int getFailCount() const;
    bool isAccountLocked() const;

private:
    void loadData();
    void handleEvents();
    void render();
    void renderFatalError(const char* message);
    void processLogin(const char* role, const char* idText, const char* passwordText);
    int nextAppointmentId() const;
    int nextBillId() const;
    int nextPrescriptionId() const;
};

#endif
