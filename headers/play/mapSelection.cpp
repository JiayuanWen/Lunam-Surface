//This is where player select a map in 'maps' folder.

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>

#include "mapSelection.h"

//If system is 32-bit or 64-bit Windows, dirent.h not included, so use a local one. 
//Credit: tronkko (GitHub). Usable under MIT License.
#ifdef _WIN32
    #include "../../outsideHeaders/dirent.h"
    #include <conio.h>
#endif

//If system is a Linux distro, dirent.h is included, so use directly. 
#ifdef __linux__
    #include <dirent.h>
    #include <sys/stat.h>
    #include <termios.h>
#endif

std::string mapSelect() {

    //If system is Windows
    #ifdef _WIN32
    //Variables
    struct dirent* entry;
    DIR* dir = opendir("maps");
    int mapCount = -1; 
    std::string mapName = "NULL";
    std::vector<std::string> mapList;
    std::ifstream mapFile;

    if (dir == NULL) {
        std::cout << "No maps found. \n";

        std::cout << "(Press 'Enter' to go back to main menu)";
        std::cin.get();
        std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );

        return ".";
    }

    //List map files in 'maps' folder
    std::cout << "<Map Selection> \n\n\n";
    std::cout << "Map List: \n";
    while ((entry = readdir(dir)) != NULL) {
        mapName = entry->d_name;
        if (mapCount > 0) { // A bug in dirent.h readdir that causes it to output two glitched file names, ignore those two names and start counting from the first real file name. 
            std::cout << mapCount << ". " << mapName << "\n";
            mapList.push_back(mapName);
        }
        mapCount++;
    }
    closedir(dir);

    //Player choose a map file
    int playerChoice = 0;

    std::cout << std::endl << "Select map (Enter the mpa number, then press 'Enter'): ";
    std::cin >> playerChoice;

    //Opening map file base on player choice
    if (playerChoice > mapCount-1 || playerChoice < 1) { // mapCount is +1 over the desired value, so mapCount-1
        std::cout << "Map file does not exist." << std::endl;
        mapFile.close();

        std::cout << "(Press 'Enter' to go back to main menu)";
        std::cin.get();
        std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );

        return ".";
    }
    else {
        mapFile.open("maps/"+mapList[playerChoice-1]);

        std::string formatString;
        std::getline(mapFile, formatString);

        //Check map file format. All map files should begin with the string "LSMap"
        if (formatString == "LSMap") {

            //For debug use. Print string from map file to see if file is read successfully
            /*
            while (std::getline(mapFile, formatString)) {
                std::cout << formatString << std::endl;
            }
            //*/

            mapFile.close();
            return ("maps/"+mapList[playerChoice-1]);
        }
        else {
            std::cout << "Map file not supported.\n";
            mapFile.close();

            std::cout << "(Press 'Enter' to go back to main menu)";
            std::cin.get();
            std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );

            return ".";
        }
    }
    #endif

    //If system is Linux
    #ifdef __linux__
    //Variables
    struct dirent* entry;
    DIR* dir;
    struct stat st;
    int mapCount = 1; 
    std::string mapName = "NULL";
    std::string directory;
    std::vector<std::string> mapList;
    std::ifstream mapFile;

    //Open "maps" folder
    directory = "maps/";
    dir = opendir(directory.c_str());

    if (dir == NULL) {
        std::cout << "No maps found. \n";
        return ".";
    }

    //List map files in 'maps' folder
    std::cout << "<Map Selection> \n \n \n";
    std::cout << "Map List: \n";
    while ((entry = readdir(dir)) != NULL) {
        mapName = entry->d_name;
        const std::string mapPath = "maps/" + mapName;

        if (mapName[0] == '.') {
            continue;
        }

        if (stat(mapPath.c_str(), &st) == -1) {
            continue;
        }

        const bool is_directory = (st.st_mode & S_IFDIR) != 0;

        if (is_directory) {
            continue;
        }

        std::cout << mapCount << ". " << mapName << std::endl;
        mapList.push_back(mapPath);

        mapCount++;
    }
    closedir(dir);

    //Player choose a map file
    int playerChoice = 0;

    std::cout <<"\nSelect map (Enter the map number, then press 'Enter'): ";
    std::cin >> playerChoice;

    //Opening map file base on player choice
    if (playerChoice > mapCount-1 || playerChoice < 1) { // mapCount is +1 over the desired value, so mapCount-1
        std::cout << "Map file does not exist.\n \n \n";
        mapFile.close();

        std::cout << "(Press 'Enter' to go back to main menu)";
        std::cin.get();
        std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );

        return ".";
    }
    else {
        mapFile.open(mapList[playerChoice-1]);

        std::string formatString;
        std::getline(mapFile, formatString);

        //Check map file format. All map files should begin with the string "LSMap"
        if (formatString == "LSMap") {

            //For debug use. Print string from map file to see if file is read successfully
            /*
            while (std::getline(mapFile, formatString)) {
                std::cout << formatString << std::endl;
            }
            //*/

            mapFile.close();
            return (mapList[playerChoice-1]);
        }
        else {
            std::cout << "Map file not supported. \n";
            mapFile.close();

            std::cout << "(Press 'Enter' to go back to main menu)";
            std::cin.get();
            std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );

            return ".";
        }
    }
    #endif

    return ".";
}