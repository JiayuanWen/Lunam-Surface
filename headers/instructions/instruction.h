//Display the game's instruction

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>
#include <iostream>
#include <limits>

void displayInstruction() {
    //Clear terminal screen for new frame
        //"\033[2J:" clears terminal from top to bottom
        //"\033[1;1H" place cursor back to top right corner, or the begining of terminal.
    std::cout << "\033[2J\033[1;1H";

    std::cout << "<Instruction> \n \n \n";

    std::cout   << "You are a lunar rover (Marked as 'T'). Your goal is to reach the base (Marked as '!'). \n \n"
                << "The moon terrian is rough, you can only drive on smooth surfaces ('~'). \n"
                << "You cannot drive on rough surfaces ('X'). \n \n \n \n";
    
    std::cout << "(Press 'Enter' to go back to main menu)";
    std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );

    return;
}

#endif