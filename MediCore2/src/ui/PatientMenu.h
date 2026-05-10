#ifndef PATIENTMENU_H
#define PATIENTMENU_H

#include "GUI.h"

class PatientMenu {
    UIButton buttons[8];
    UITextField specField;
    UITextField dateField;
    UITextField topupField;
    UITable table;
    int topupFailCount;
    char feedback[220];

public:
    PatientMenu(const sf::Font& font);
    ~PatientMenu();

    void handleEvent(const sf::Event& event, GUI& gui);
    void updateHover(sf::Vector2i mousePos);
    void draw(sf::RenderWindow& window, GUI& gui);
};

#endif
