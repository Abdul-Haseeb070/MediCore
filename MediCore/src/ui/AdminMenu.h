#ifndef ADMINMENU_H
#define ADMINMENU_H

#include "GUI.h"

class AdminMenu {
    UIButton buttons[10];
    UITextField addDoctorFields[5];
    UITextField removeDoctorField;
    UITextField dischargeField;
    UITable table;
    char feedback[240];

public:
    AdminMenu(const sf::Font& font);
    ~AdminMenu();

    void handleEvent(const sf::Event& event, GUI& gui);
    void updateHover(sf::Vector2i mousePos);
    void draw(sf::RenderWindow& window, GUI& gui);
};

#endif
