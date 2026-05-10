#ifndef DOCTORMENU_H
#define DOCTORMENU_H

#include "GUI.h"

class DoctorMenu {
    UIButton buttons[6];
    UITextField appointmentIdField;
    UITextField medicinesField;
    UITextField notesField;
    UITextField historyPatientField;
    UITable table;
    char feedback[220];

public:
    DoctorMenu(const sf::Font& font);
    ~DoctorMenu();

    void handleEvent(const sf::Event& event, GUI& gui);
    void updateHover(sf::Vector2i mousePos);
    void draw(sf::RenderWindow& window, GUI& gui);
};

#endif
