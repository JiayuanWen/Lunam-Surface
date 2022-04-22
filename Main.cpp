#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

#include "headers/setting/customKeybind.h"
#include "headers/play/mapSelection.h"

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

//Function prototypes for avoiding "Not declared in scope" errors.
void mainMenu();

//----------------Main driver----------------
int main() {

    mapSelect();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    return 0;
}

//--------------Menu menu----------------

void mainMenu() {
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

        while (session == true) {
            //Clear terminal screen for new frame
                //"\033[2J:" clears terminal from top to bottom
                //"\033[1;1H" place cursor back to top right corner, or the begining of terminal.
            std::cout << "\033[2J\033[1;1H" << std::endl;

            //For debug. Check if system got user input.
            //std::cout << input << std::endl;

            std::cout << "    XXXXXXXXX~~~~~XXXXXXXXXXXX~~XXXXXXXXXXXXXXXX  " << std::endl;
            std::cout << "        _____                                     " << std::endl;
            std::cout << "       |     |_.--.--.-----.---.-.--------.       " << std::endl;
            std::cout << "       |       |  |  |     |  _  |        |       " << std::endl;
            std::cout << "       |_______|_____|__|__|___._|__|__|__|       " << std::endl;
            std::cout << "    _______              ___                      " << std::endl;
            std::cout << "    |     __|.--.--.----.'  _|.---.-.----.-----.  " << std::endl;
            std::cout << "    |__     ||  |  |   _|   _||  _  |  __|  -__|  " << std::endl;
            std::cout << "    |_______||_____|__| |__|  |___._|____|_____|  " << std::endl;
            std::cout << "                                                  " << std::endl;
            std::cout << "    XX~~~~~~~~XXXXXX~~XXXXXXXXXXXX~~~~~XXXXXXXXX  " << std::endl;
            std::cout << "                                                  " << std::endl;
            std::cout << "                                                  " << std::endl;
            

            //Get input
            input = getchar();

            //Process input
            switch (input) {
                case 'w':
                    
                    break;
                case 's':

                    break;
                case '\n':
                    
                    break;
                default:
                    std::cout << "";
            }
        }
    #endif
}


