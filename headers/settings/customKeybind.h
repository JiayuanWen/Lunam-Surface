//This is where players change control schemes
//This is where the game loads control schemes

//Probably the most painful header I worked on. Good luck to those who want to modify this...

#ifndef CUSTOMKEYBIND_H
#define CUSTOMKEYBIND_H

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cstdio>

//Control schemes. They are capital but will be converted to lower case for the game. 
char up;
char down;
char left;
char right;

//Variables
std::ifstream keybind;
std::ofstream keybind_2;
std::string settingLoc;
std::string settingLoc_temp;
std::string temp;
std::string input;
char c;
bool session;
bool continue_;

//Function prototypes for avoiding "Not declared in scope" errors.
void changeSetting(char setting);

void confirmKeybind() {
    //Variables
    std::ifstream keybind;
    std::string line;

    //Open settings file
    keybind.open("settings/keyBindings.txt");

    //Assign controls
    getline(keybind,line);
    up = line.back();
    tolower(up);

    getline(keybind,line);
    down = line.back();
    tolower(down);

    getline(keybind,line);
    left = line.back();
    tolower(left);

    getline(keybind,line);
    right = line.back();
    tolower(right);

    return;
}

char loadKeybind(std::string control) {
    if (control == "up") {
        return tolower(up);
    }
    else if (control == "down") {
        return tolower(down);
    }
    else if (control == "left") {
        return tolower(left);
    }
    else if (control == "right") {
        return tolower(right);
    }
    return NULL;
}

void changeKeybind() {
    //Open settings file
    settingLoc = "settings/keyBindings.txt";
    keybind.open(settingLoc);
    settingLoc_temp = "settings/keyBindings_temp.txt";
    keybind_2.open(settingLoc_temp);

    session = true;
    continue_ = true;

    if (keybind && keybind_2) {
        /*
        while (getline(keybind, temp)) {
            keybind_2_temp << temp << "\n";
        }
        */
    }
    else {
        std::cout << "Error: Settings file not found. \n \n \n";

        std::cout << "(Press 'Enter' to go back to main menu)";
        std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );

        return;
    }

    while (session) {
        //Clear terminal screen for new frame
            //"\033[2J:" clears terminal from top to bottom
            //"\033[1;1H" place cursor back to top right corner, or the begining of terminal.
        std::cout << "\033[2J\033[1;1H";

        //Reset file read
        keybind.close();
        keybind.open(settingLoc);

        std::cout << "<Settings> \n \n";

        std::cout << "Controls \n------------------------------- \n";
        getline(keybind,temp);
        std::cout << "Up:       " << up << std::endl;
        getline(keybind,temp);
        std::cout << "Down:     " << down << std::endl;
        getline(keybind,temp);
        std::cout << "Left:     " << left << std::endl;
        getline(keybind,temp);
        std::cout << "Right:    " << right << std::endl;

        std::cout << std::endl << std::endl << std::endl;

        std::cout << "Choose a setting to change \n('U' for Up, 'D' for Down, 'L' for Left, 'R' for Right. 'Q' to exit. 'Enter' to confirm): ";
        std::cin >> input;

        switch (input[0]) { //Take the first character of player input. In case player typed a sentence instead.
            case 'U':
            case 'u':
                changeSetting(up);
                break;
            case 'D':
            case 'd':
                changeSetting(down);

                break;
            case 'L':
            case 'l':
                changeSetting(left);

                break;
            case 'R':
            case 'r':
                changeSetting(right);

                break;
            default:
                session = false;
        }
        
    }

    keybind.close();
    keybind_2.close();
    remove(settingLoc_temp.c_str());

    confirmKeybind();

    return;
}

void changeSetting(char setting) {
    std::cout << "\nChange the setting to: ";
    std::cin >> input;

    //Reset file read
    keybind.close();
    keybind.open(settingLoc);

    //Check if input is in use
    for(int i=0; !keybind.eof(); i++) {
        c = keybind.get();
        if(toupper(input[0]) == c) {
            std::cout << "\n\nKey is already in-use, try another key. \n(Press 'Enter' to continue)";
            std::cin.get();
            std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );

            continue_ = false;
            break;
        }
        continue_ = true;
    }
                

    if (continue_ == true) {
        //Reset file read
        keybind.close();
        keybind.open(settingLoc);

        //Copy settings file with new changes to temp string
        temp = "";
        for(int i=0; !keybind.eof(); i++) {
            c = keybind.get();
            if(c == toupper(setting)) {
                c = toupper(input[0]);
            }
            temp += c;
        }
        temp.erase(temp.end()-1); //Erase invalid character at the end   

        //Copy temp string to temp settings file
        keybind_2 << temp;
        keybind_2.close();
        keybind.close();

        //Copy temp settings file to temp string
        keybind.open(settingLoc_temp);
        temp = "";
        for(int i=0; !keybind.eof(); i++) {
            c = keybind.get();
            if(c == toupper(setting)) {
                c = toupper(input[0]);
            }
            temp += c;
        }
        temp.erase(temp.end()-1); //Erase invalid character at the end   
        keybind.close();

        //Copy temp string to settings file
        keybind_2.open(settingLoc);
        keybind_2 << temp;
        keybind_2.close();

        //Assign new changes to game.
        confirmKeybind();
    }
}

#endif