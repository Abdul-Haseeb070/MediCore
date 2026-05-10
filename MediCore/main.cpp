// Class: main  

#include "src/ui/GUI.h"

/// @brief Create and run app.
/// @return Process exit code.
int launchApplication() {
    GUI gui;
    gui.run();
    return 0;
}

/// @brief Program entry.
/// @return Process exit code.
int main() {
    return launchApplication();
}
