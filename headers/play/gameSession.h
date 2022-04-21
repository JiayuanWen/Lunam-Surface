#ifndef GAMESESSION_H
#define GAMESESSION_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// The following is for capturing player input without player needing to press 'Enter'
    //If system is 32-bit or 64-bit Windows, use conio.h.
#ifdef _WIN32
    #include <conio.h>
    #include <Windows.h>
#endif

    //If system is a Linux distro, use termios.h.
#ifdef linux
    #include <termios.h>
    #include <unistd.h>
    #include <signal.h>

    //Capturing player input without prompt require editing terminal settings. Use global variables to store settings.
    struct termios new_tio; //new_tio is tempory terminal settings for the game. 
    struct termios old_tio; //old_tio is backup of old settings to be restored after gamed ends or interrupted.

    //Interrupt signal handler. For restoring terminal settings if game is interrupted. 
    static void restoreSettings(int signum)
    {
        std::cout << "(Signal:" << signum << ") Lunam Surface force closed.";
        (void)signum;
        tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);
    }
#endif

//Map
std::vector<std::vector<char>> map;
std::vector<std::vector<char>> map_backup;

//Player
int posX = 0; 
int posY = 0; 
int energy = 100; //Player energy from 100 to 0

//Signaling between files
bool ready = false;

//Import map from mapConvert.h , then place player at starting point.
void startSection(std::vector<std::vector<char>> map_import) {
    //Import map from mapConvert.h
    map = map_import;
    map_backup = map_import;

    //Find '^' in map, that is the starting point.
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            //std::cout << map[y][x]; //For Debug, checking if map is still intact.
            if (map[y][x] == '^') {
                posX = x;
                posY = y;
                map[y][x] = 'T';
            }
        }
        //std::cout << "\n";
    }

    /*
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            std::cout << map[y][x]; //For Debug, checking if player "T" is on map.
        }
        std::cout << "\n";
    }
    //*/
    /*
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            std::cout << map[y][x]; //For Debug, checking if backup map is still intact.
        }
        std::cout << "\n";
    }
    //*/

    //Start the game.
    game();
}

//
void game() {
    // If system is Linux
    #ifdef linux
    //Variables
    char input;

    //Setting up interrupt signal handler
    signal(SIGINT, restoreSettings);

    //Change terminal settings for real-time player input
    //Get terminal settings from stdin
    tcgetattr(STDIN_FILENO, &new_tio);
    //Get a copy of settings for restoration.
    old_tio = new_tio;
    //Disable canonical mode (So players no longer need to press 'Enter' after input)
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    

    #endif

    //If system is Windows
    #ifdef _WIN32

    #endif
}

#endif