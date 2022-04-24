//Game session header. The game is rendered and played here. 

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
#ifdef __linux__
    #include <termios.h>
    #include <unistd.h>
    #include <signal.h>

    //Capturing player input without prompt require editing terminal settings. Use global variables to store settings.
    struct termios new_tio; //new_tio is tempory terminal settings for the game. 
    struct termios old_tio; //old_tio is backup of old settings to be restored after gamed ends or interrupted.

    //Interrupt signal handler. For restoring terminal settings if game is interrupted. 
    static void restoreSettings(int signum)
    {
        (void)signum;
        tcsetattr(STDIN_FILENO,TCSANOW,&old_tio); std::cout << "Terminal settings reset." << std::endl;

        std::cout << "(Signal:" << signum << ") Lunam Surface force closed." << std::endl;
        exit(-1);
    }
#endif

//Map
std::vector<std::vector<char>> map;
std::vector<std::vector<char>> map_overlap;

//Player
int posX = 0; 
int posY = 0; 
int energy = 100; //Player energy from 100 to 0

//Controls;
char UP = 'w';
char DOWN = 's';
char LEFT = 'a';
char RIGHT = 'd';

//Function prototypes for avoiding "Not declared in scope" errors.
void gameSession();
void moveUP();
void moveDOWN();
void moveLEFT();
void moveRIGHT();

//Player movement reference
/*      <----- posX--        posX++ ----->
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
XXXXXXX~XXXXXXXXXXXXXXXXXXX~~~~~~~~~~~~XXXXXX~~~~!
X~~~~~~~~~~~~~~XXXXX~XXXXXX~XXXXXXXXXX~~~~~~~~XXXX  ^
XXX XXXXXXXXXX XXXXX~XXXXXX~XXXXXXXXXXXXXXXXXXXXXX  |
XXX~~~~~~~XXXX~~~~~~~XXXX~~~~~~~~~~~~~~~~~~~XXXXXX  posY--
XX~XXXXXX~XXXXXXXXXXXXXXXXXXXXXXXXX~XXXXXXX~XXXXXX  
XX~XXXXXX~~~~~~~~~~~XXXXXXXXXX~XXXX~~~~~~~~~XXXXXX  posY++
X~~~~~~~~~XXXXXXXXX~XXXXXXXXXX~XXXX~XXXXXXXXXXXXXX  |
XXXXXXXXX~XXXXXXXXX~~~T~~~~~~X~XXX~~~~~~~~~~XXXXXX  V
XX~~~~~~~~~~~~~~XXXXXXXXXXXX~XX~~~XXX~XXXXX~XXXXXX
XX~XXXXXXXXXXXX~XX XXXXXXXXX~XXXX~XXX~XXXXX~XXXXXX
XX~XXX~XXXXXXXX~XX~~~~~~~~~~~XXXX~XXX~XXXXX~XXXXXX
XX~XXX~XXXXXXXX~XXXXXXX~XXXXXXXXX~XXX~~~~XX~XXXXXX
XX~XXX~~~~~~~~~~XXXXXXX~XXXXXXXXX~XXXXXXXXX~XXXXXX
XX~XXX~XXXX~XXXXX~~~~~~~~~~~~~~~~~~~~~XXXXX~XXXXXX
XX~XXX~XXXX~XXXXXXXXXXX~XXXXXXXXXXXXX~XXXXX~XXXXXX
XXXXXX~XXXX~~~~~~~~XXXX~XXXXX~~~~~~~~~~~~~~~~~~XXX
XXXXXX~XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  
//Don't ask me why Y-axis is inverted and map is accessed via map[y][x] instead of map[x][y], it's the nature of 2D array.
*/

//Import map from mapConvert.h , then place player at starting point.
void preSection(std::vector<std::vector<char>> map_import) {
    //Import map from mapConvert.h
    map = map_import;
    map_overlap = map_import;

    //Find '^' in map, that is the starting point.
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            //std::cout << map[y][x]; //For Debug, checking if map is still intact.
            if (map[y][x] == '^') {
                posX = x;
                posY = y;
                map[y][x] = 'T';
                //map_overlap[y][x] = 'X';
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

    //Start the game.
    gameSession();

    return;
}

//
void gameSession() {
    // If system is Linux
    #ifdef __linux__
        //Variables
        char input = 'n';
        bool session = true;

        //Setting up interrupt signal handler
        signal(SIGINT, restoreSettings);

        //Change terminal settings for real-time player input
            //Get terminal settings from stdin
        tcgetattr(STDIN_FILENO, &new_tio);
            //Get a copy of settings for restoration.
        old_tio = new_tio;
            //Disable canonical mode (So players no longer need to press 'Enter' after input)
        new_tio.c_lflag &=(~ICANON & ~ECHO);
            //Save and use new terminal settings. 
        tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

        //Game session
        while (session == true) {
            //Clear terminal screen for new frame
                //"\033[2J:" clears terminal from top to bottom
                //"\033[1;1H" place cursor back to top right corner, or the begining of terminal.
            std::cout << "\033[2J\033[1;1H" << std::endl;

            //For debug. Check if system got user input.
            //std::cout << input << std::endl;

            //Print frame
            for (int y = 0; y < map.size(); y++) {
                for (int x = 0; x < map[y].size(); x++) {
                    std::cout << map[y][x]; 
                }
                std::cout << "\n";
            }

            //Get input
            input = getchar();

            //Process input
            switch (input) {
                case 'w':
                    moveUP();
                    break;
                case 's':
                    moveDOWN();
                    break;
                case 'a':
                    moveLEFT();
                    break;
                case 'd':
                    moveRIGHT();
                    break;
                default:
                    break;
            }

            //Player reached base?
            switch (map_overlap[posY][posX]) {
                case '!':
                    session = false;
                    return;
                default:
                    break;
            }
        }
    #endif

    //If system is Windows
    #ifdef _WIN32

    #endif

    return;
}

//Player movements
void moveUP() {
    //Only proceed if player is inside map and next move not land on 'X'
    if (posY > 0 && map[posY-1][posX] != 'X') {
        //Remove player from old location. 
        map[posY][posX] = map_overlap[posY][posX]; 
        //Move player position up 1 unit
        posY--;
        //Place player to new location. 
        map[posY][posX] = 'T';
    }

    return;
}
void moveDOWN() {
    //Only proceed if player is inside map and next move not land on 'X'
    if (posY < map.size()-1 && map[posY+1][posX] != 'X') {
        //Remove player from old location. 
        map[posY][posX] = map_overlap[posY][posX]; 
        //Move player position down 1 unit
        posY++;
        //Place player to new location. 
        map[posY][posX] = 'T';
    }

    return;
}
void moveLEFT() {
    //Only proceed if player is inside map and next move not land on 'X'
    if (posX > 0 && map[posY][posX-1] != 'X') {
        //Remove player from old location. 
        map[posY][posX] = map_overlap[posY][posX]; 
        //Move player position left 1 unit
        posX--;
        //Place player to new location. 
        map[posY][posX] = 'T';
    }

    return;
}
void moveRIGHT() {
    //Only proceed if player is inside map and next move not land on 'X'
    if (posX < map[posY].size()-1 && map[posY][posX+1] != 'X') {
        //Remove player from old location. 
        map[posY][posX] = map_overlap[posY][posX]; 
        //Move player position right 1 unit
        posX++;
        //Place player to new location. 
        map[posY][posX] = 'T';
    }

    return;
}

#endif