// Class: LoginScreen  

#include "LoginScreen.h"
#include "../utils/MyString.h"

namespace {
const sf::Color BG(10, 22, 40);
const sf::Color CARD(20, 35, 60);
const sf::Color ACCENT(0, 212, 200);
const sf::Color TEXT_PRIMARY(240, 244, 255);
const sf::Color TEXT_MUTED(140, 160, 190);
const sf::Color ERROR_RED(255, 80, 80);
}

LoginScreen::LoginScreen(const sf::Font& font)
    : patientBtn(font, "Patient", 390.0f, 250.0f, 160.0f, 50.0f, 20u),
      doctorBtn(font, "Doctor", 560.0f, 250.0f, 160.0f, 50.0f, 20u),
      adminBtn(font, "Admin", 730.0f, 250.0f, 160.0f, 50.0f, 20u),
      loginBtn(font, "Login", 390.0f, 450.0f, 500.0f, 52.0f, 22u),
      idField(font, 390.0f, 330.0f, 500.0f, 48.0f, 20, false),
      passField(font, 390.0f, 390.0f, 500.0f, 48.0f, 30, true) {
    myStrcpy(selectedRole, "Patient");
}

LoginScreen::~LoginScreen() {}

void LoginScreen::updateHover(sf::Vector2i mousePos) {
    patientBtn.updateHover(mousePos);
    doctorBtn.updateHover(mousePos);
    adminBtn.updateHover(mousePos);
    loginBtn.updateHover(mousePos);
}

void LoginScreen::handleEvent(const sf::Event& event, GUI& gui) {
    if (gui.isAccountLocked()) {
        return;
    }

    if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2i pos = mouse->position;

        idField.setFocused(idField.contains(pos));
        passField.setFocused(passField.contains(pos));

        if (patientBtn.handleClick(pos)) {
            myStrcpy(selectedRole, "Patient");
        }
        if (doctorBtn.handleClick(pos)) {
            myStrcpy(selectedRole, "Doctor");
        }
        if (adminBtn.handleClick(pos)) {
            myStrcpy(selectedRole, "Admin");
        }

        if (loginBtn.handleClick(pos)) {
            gui.requestLogin(selectedRole, idField.getValue(), passField.getValue());
        }
    }

    if (const auto* text = event.getIf<sf::Event::TextEntered>()) {
        idField.handleText(text->unicode);
        passField.handleText(text->unicode);

        if (text->unicode == 10) {
            gui.requestLogin(selectedRole, idField.getValue(), passField.getValue());
        }
    }
}

void LoginScreen::draw(sf::RenderWindow& window, const sf::Font& font, int failCount, bool accountLocked) {
    sf::Text title(font, "MediCore", 64u);
    title.setFillColor(ACCENT);
    title.setPosition({470.0f, 90.0f});

    sf::Text subtitle(font, "Hospital Management System", 24u);
    subtitle.setFillColor(TEXT_MUTED);
    subtitle.setPosition({448.0f, 165.0f});

    sf::Text idLabel(font, "User ID", 18u);
    idLabel.setFillColor(TEXT_PRIMARY);
    idLabel.setPosition({390.0f, 305.0f});

    sf::Text passLabel(font, "Password", 18u);
    passLabel.setFillColor(TEXT_PRIMARY);
    passLabel.setPosition({390.0f, 365.0f});

    patientBtn.setSelected(myStrcmp(selectedRole, "Patient") == 0);
    doctorBtn.setSelected(myStrcmp(selectedRole, "Doctor") == 0);
    adminBtn.setSelected(myStrcmp(selectedRole, "Admin") == 0);

    patientBtn.draw(window);
    doctorBtn.draw(window);
    adminBtn.draw(window);

    idField.draw(window);
    passField.draw(window);
    loginBtn.draw(window);

    window.draw(title);
    window.draw(subtitle);
    window.draw(idLabel);
    window.draw(passLabel);

    char attempt[50];
    attempt[0] = '\0';
    myStrcpy(attempt, "Attempt ");
    char countBuf[8];
    myItoa(failCount, countBuf);
    myStrcat(attempt, countBuf);
    myStrcat(attempt, "/3");

    sf::Text failText(font, attempt, 18u);
    failText.setFillColor(ERROR_RED);
    failText.setPosition({390.0f, 510.0f});
    window.draw(failText);

    if (accountLocked) {
        sf::RectangleShape overlay({1280.0f, 720.0f});
        overlay.setFillColor(sf::Color(80, 80, 80, 180));

        sf::RectangleShape card({620.0f, 140.0f});
        card.setPosition({330.0f, 290.0f});
        card.setFillColor(CARD);
        card.setOutlineColor(ERROR_RED);
        card.setOutlineThickness(2.0f);

        sf::Text locked(font, "Account Locked. Contact Admin.", 30u);
        locked.setFillColor(ERROR_RED);
        locked.setPosition({370.0f, 338.0f});

        window.draw(overlay);
        window.draw(card);
        window.draw(locked);
    }
}
