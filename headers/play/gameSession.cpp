//Game session header declaration. The game is rendered and played here. 

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>

#include "../../headers/settings/customKeybind.h"
#include "mapSelection.h"
#include "mapConvert.h"
#include "gameSession.h"

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
int posX; 
int posY; 
int moves;
std::chrono::high_resolution_clock::time_point start_timer, end_timer;
std::chrono::duration<double, std::milli> time_;

//Controls
char UP;
char DOWN;
char LEFT;
char RIGHT;

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
void preSession() {
    //Player select a map
    std::string mapName = mapSelect();
    //End the session and return to main menu if map doesn't exist or failed to load.
    if (mapName == ".") {
        return;
    }
    //Convert map to 2D vector for the game.
    map = mapToVector(mapName);
    map_overlap = map;
    //In case the conversion is unsuccessful or map file somehow missing, end session and return to main menu.
    if (map.empty()) {
        return;
    }

    //Reset player stats
    moves = 0;

    //Find '^' in map, that is the starting point.
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            if (map[y][x] == '^') {
                posX = x;
                posY = y;
                map[y][x] = 'T';
                //map_overlap[y][x] = 'X';
            }
        }
    }

    //Start the game.
    gameSession();
    return;
}

//Game session
void gameSession() {
    //Load control schemes from setting
    UP = loadKeybind("up");
    DOWN = loadKeybind("down");
    LEFT = loadKeybind("left");
    RIGHT = loadKeybind("right");

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

        //Start timer
        start_timer = std::chrono::high_resolution_clock::now();

        //Game session
        while (session == true) {
            
            //Clear terminal screen for new frame
                //"\033[2J:" clears terminal from top to bottom
                //"\033[1;1H" place cursor back to top right corner, or the begining of terminal.
            std::cout << "\033[2J\033[1;1H";

            //For debug. Check if system got user input.
            //std::cout << input << std::endl;

            //Print frame
            for (int y = 0; y < map.size(); y++) {
                for (int x = 0; x < map[y].size(); x++) {
                    std::cout << map[y][x]; 
                }
                std::cout << "\n";
            }
            std::cout << "\n" << "Moves: " << moves << "\n";

            //Get input
            input = getchar();

            //Process input
            if (input == UP) {
                moveUP();
            }
            else if (input == DOWN) {
                moveDOWN();
            }
            else if (input == LEFT) {
                moveLEFT();
            }
            else if (input == RIGHT) {
                moveRIGHT();
            }

            //Player reached base?
            switch (map_overlap[posY][posX]) {
                case '!':
                    //Stop timer
                    end_timer = std::chrono::high_resolution_clock::now();
                    time_ = end_timer - start_timer;
                    //Display Game Over screen w/ player stats
                    gameOver();
                    //Press any key to continue
                    input = getchar();
                    //End game session
                    session = false;
                    return;

                default:
                    break;
            }
        }
    #endif

    //If system is Windows
    #ifdef _WIN32
        //Variables
        char input = 'n';
        bool session = true;

        //Start timer
        start_timer = std::chrono::high_resolution_clock::now();

        //Game session
        while (session == true) {
            
            //Clear terminal screen for new frame
            system("cls");

            //For debug. Check if system got user input.
            //std::cout << input << std::endl;

            //Print frame
            for (int y = 0; y < map.size(); y++) {
                for (int x = 0; x < map[y].size(); x++) {
                    std::cout << map[y][x]; 
                }
                std::cout << "\n";
            }
            std::cout << "\n" << "Moves: " << moves << "\n";

            //Get input
            input = _getch();

            //Process input
            if (input == UP) {
                moveUP();
            }
            else if (input == DOWN) {
                moveDOWN();
            }
            else if (input == LEFT) {
                moveLEFT();
            }
            else if (input == RIGHT) {
                moveRIGHT();
            }

            //Player reached base?
            switch (map_overlap[posY][posX]) {
                case '!':
                    //Stop timer
                    end_timer = std::chrono::high_resolution_clock::now();
                    time_ = end_timer - start_timer;
                    //Display Game Over screen w/ player stats
                    gameOver();
                    //Clear input buffer. This is to prevent the game from skipping the game over screen.
                    while ((getchar()) != '\n');
                    //Press any key to continue
                    input = _getch();
                    //End game session
                    session = false;
                    return;

                default:
                    break;
            }
        }
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
        //Add move counter
        moves++;
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
        //Add move counter
        moves++;
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
        //Add move counter
        moves++;
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
        //Add move counter
        moves++;
    }

    return;
}

void gameOver() {

    #ifdef __linux__
    //Clear terminal screen for new frame
        //"\033[2J:" clears terminal from top to bottom
        //"\033[1;1H" place cursor back to top right corner, or the begining of terminal.
    std::cout << "\033[2J\033[1;1H";
    #endif
    #ifdef _WIN32
    //Clear terminal screen for new frame
    system("cls");
    #endif

    std::cout   << "    XX~~~~~~~~XXXXXX~~XXXXXXXXXXXX~~~~~XXXXXXXXX  \n"
                << "               < You Reached The Base! >          \n"
                << "    XXXXXXXXX~~~~~XXXXXXXXXXXX~~XXXXXXXXXXXXXXXX  \n"
                << "\n\n"
                << "              Moves: " << moves << "\n"
                << "              Time: " << time_.count() * 0.001 << " seconds" << "\n"
                << "\n\n\n"
                << "(Press any key to go back to main menu)";

    return;
}