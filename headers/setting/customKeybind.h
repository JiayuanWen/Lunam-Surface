#ifndef CUSTOMKEYBIND_H
#define CUSTOMKEYBIND_H

#include <iostream>
#include <fstream>
#include <string>

void changeKeybind() {
    char up = 'N';
    char down = 'N';
    char left = 'N';
    char right = 'N';
    std::ifstream keybind("menuOptions/Keybindings/keyBindings.txt");
    std::string temp = "NULL";

    while (getline(keybind, temp)) {
        std::cout << temp << std::endl;
    }
    keybind.close();
    return;
}
#endif