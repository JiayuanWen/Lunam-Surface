//Display the game's instruction

#include <string>
#include <iostream>
#include <limits>

void displayAbout() {
    //Clear terminal screen for new frame
        //"\033[2J:" clears terminal from top to bottom
        //"\033[1;1H" place cursor back to top right corner, or the begining of terminal.
    std::cout << "\033[2J\033[1;1H";

    std::cout << "<About> \n \n";

    std::cout   << "Lunam Surface \n \n \n"
                << "Designed, coded by Jiayuan Wen \n \n \n"
                << "Version: 0.2.0 \n"
                << "GitHub: https://github.com/JiayuanWen/Lunam-Surface\n \n \n \n";
    
    std::cout << "(Press 'Enter' to go back to main menu)";

    std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );

    return;
}