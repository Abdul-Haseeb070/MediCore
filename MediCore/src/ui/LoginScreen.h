#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include "GUI.h"

class LoginScreen {
    UIButton patientBtn;
    UIButton doctorBtn;
    UIButton adminBtn;
    UIButton loginBtn;
    UITextField idField;
    UITextField passField;
    char selectedRole[20];

public:
    LoginScreen(const sf::Font& font);
    ~LoginScreen();

    void handleEvent(const sf::Event& event, GUI& gui);
    void updateHover(sf::Vector2i mousePos);
    void draw(sf::RenderWindow& window, const sf::Font& font, int failCount, bool accountLocked);
};

#endif
