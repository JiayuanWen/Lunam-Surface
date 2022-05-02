#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

#include "headers/play/mapSelection.h"
#include "headers/instructions/instruction.h"
#include "headers/settings/customKeybind.h"

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
    struct termios new_tio_; //new_tio is tempory terminal settings for the game. 
    struct termios old_tio_; //old_tio is backup of old settings to be restored after gamed ends or interrupted.

    //Interrupt signal handler. For restoring terminal settings if game is interrupted. 
    static void restoreSetting(int signum)
    {
        (void)signum;
        tcsetattr(STDIN_FILENO,TCSANOW,&old_tio_); std::cout << "Terminal settings reset." << std::endl;

        std::cout << "(Signal:" << signum << ") Lunam Surface force closed." << std::endl;
        exit(-1);
    }
#endif

//Function prototypes for avoiding "Not declared in scope" errors.
void mainMenu();

//----------------Main driver----------------
int main() {

    mainMenu();
    //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    return 0;
}

//--------------Menu menu----------------

void mainMenu() {
    // If system is Linux
    #ifdef __linux__
        //Variables
        char input = 'n';
        bool session = true;
        bool backToMenu = true;
        short int option = 1;

        //Setting up interrupt signal handler
        signal(SIGINT, restoreSetting);

        //Assign control schemes from file
        confirmKeybind();

        //Get terminal settings from stdin
        tcgetattr(STDIN_FILENO, &new_tio_);
        //Get a copy of settings for restoration.
        old_tio_ = new_tio_;

        while (session == true) {

            //If player back to main menu
            if (backToMenu == true) {
                //Change terminal settings for real-time player input
                    //Disable canonical mode (So players no longer need to press 'Enter' after input)
                new_tio_.c_lflag &=(~ICANON & ~ECHO);
                    //Save and use new terminal settings. 
                tcsetattr(STDIN_FILENO, TCSANOW, &new_tio_);

                //Player is currently in menu
                backToMenu = false;
            }


            //Clear terminal screen for new frame
                //"\033[2J:" clears terminal from top to bottom
                //"\033[1;1H" place cursor back to top right corner, or the begining of terminal.
            std::cout << "\033[2J\033[1;1H" << std::endl;

            //For debug. Check if system got user input.
            //std::cout << input << std::endl;

            //Main menu
                std::cout   << "    XXXXXXXXX~~~~~XXXXXXXXXXXX~~XXXXXXXXXXXXXXXX  \n"
                            << "        _____                                     \n"
                            << "       |     |_.--.--.-----.---.-.--------.       \n"
                            << "       |       |  |  |     |  _  |        |       \n"
                            << "       |_______|_____|__|__|___._|__|__|__|       \n"
                            << "    _______              ___                      \n"
                            << "    |     __|.--.--.----.'  _|.---.-.----.-----.  \n"
                            << "    |__     ||  |  |   _|   _||  _  |  __|  -__|  \n"
                            << "    |_______||_____|__| |__|  |___._|____|_____|  \n"
                            << "                                                  \n"
                            << "    XX~~~~~~~~XXXXXX~~XXXXXXXXXXXX~~~~~XXXXXXXXX  \n";

            if (option == 1) {
                std::cout   << "                                                  \n"
                            << "                     > Start <                    \n"
                            << "                                                  \n"
                            << "                    Instruction                   \n"
                            << "                                                  \n"
                            << "                      Settings                    \n"
                            << "                                                  \n"
                            << "                        Quit                      \n"
                            << "                                                  \n"
                            << "                                                  \n"
                            << "                                                  \n"
                            << "('W' and 'S' to Choose. 'L' to Select)";
            }
            else if (option == 2) {
                std::cout   << "                                                  \n"
                            << "                       Start                      \n"
                            << "                                                  \n"
                            << "                  > Instruction <                 \n"
                            << "                                                  \n"
                            << "                      Settings                    \n"
                            << "                                                  \n"
                            << "                        Quit                      \n"
                            << "                                                  \n"
                            << "                                                  \n"
                            << "                                                  \n"
                            << "('W' and 'S' to Choose. 'L' to Select)";
            }
            else if (option == 3) {
                std::cout   << "                                                  \n"
                            << "                       Start                      \n"
                            << "                                                  \n"
                            << "                    Instruction                   \n"
                            << "                                                  \n"
                            << "                    > Settings <                  \n"
                            << "                                                  \n"
                            << "                        Quit                      \n"
                            << "                                                  \n"
                            << "                                                  \n"
                            << "                                                  \n"
                            << "('W' and 'S' to Choose. 'L' to Select)";
            }
            else if (option == 4) {
                std::cout   << "                                                  \n"
                            << "                       Start                      \n"
                            << "                                                  \n"
                            << "                    Instruction                   \n"
                            << "                                                  \n"
                            << "                      Settings                    \n"
                            << "                                                  \n"
                            << "                      > Quit <                    \n"
                            << "                                                  \n"
                            << "                                                  \n"
                            << "                                                  \n"
                            << "('W' and 'S' to Choose. 'L' to Select)";
            }

            //Get input
            input = getchar();

            //Process input
            switch (input) {
                case 'w':
                    if (option != 1) {
                        option--;
                    }
                    break;
                case 's':
                    if (option != 4) {
                        option++;
                    }
                    break;
                case 'l':
                    if (option == 1) { //Start
                        tcsetattr(STDIN_FILENO,TCSANOW,&old_tio_); //Reset terminal setting
                        std::cout << "\033[2J\033[1;1H" << std::endl; //Clear screen
                        mapSelect(); //Go to map selection
                        backToMenu = true;
                    }
                    else if (option == 2) { //Instructions
                        tcsetattr(STDIN_FILENO,TCSANOW,&old_tio_); //Reset terminal setting
                        displayInstruction();
                        backToMenu = true;
                    }
                    else if (option == 3) { //Settings
                        tcsetattr(STDIN_FILENO,TCSANOW,&old_tio_); //Reset terminal setting
                        changeKeybind();
                        backToMenu = true;
                    }
                    else if (option == 4) { //Quit
                        tcsetattr(STDIN_FILENO,TCSANOW,&old_tio_); //Reset terminal setting
                        std::cout << "\033[2J\033[1;1H" << std::endl; //Clear screen
                        session = false;
                        backToMenu = false;
                    }
                    else {
                        std::cout << "\033[2J\033[1;1H" << std::endl; //Clear screen
                    }
                    break;
                default:
                    break;
            }
        }
    #endif

    return;
}


