//This is where player select a map in 'maps' folder.

#ifndef MAPSELECTION_H
#define MAPSELECTION_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "mapConvert.h"

//If system is 32-bit or 64-bit Windows, dirent.h not included, so use a local one. 
//Credit: tronkko (GitHub). Usable under MIT License.
#ifdef _WIN32
    #include "../../outsideHeaders/dirent.h"
    #include <conio.h>
#endif

//If system is a Linux distro, dirent.h is included, so use directly. 
#ifdef linux
    #include <dirent.h>
    #include <termios.h>
#endif

void mapSelect() {

    //Variables
    struct dirent* entry;
    DIR* dir = opendir("maps");
    int mapCount = -1; 
    std::string mapName = "NULL";
    std::vector<std::string> mapList;
    std::ifstream mapFile;

    //List map files in 'maps' folder
    if (dir == NULL) {
        std::cout << "No maps found." << std::endl;
        return;
    }
    std::cout << "Map List: " << std::endl;
    while ((entry = readdir(dir)) != NULL) {
        mapName = entry->d_name;
        if (mapCount > 0) { // A bug in dirent.h readdir that causes it to output two glitched file names, ignore those two names and start counting from the first real file name. 
            std::cout << mapCount << ". " << mapName << std::endl;
            mapList.push_back(mapName);
        }
        mapCount++;
    }
    closedir(dir);

    //Player choose a map file
    int playerChoice = 0;

    std::cout << std::endl << "Select map: " << std::endl;
    std::cin >> playerChoice;

    //Opening map file base on player choice
    if (playerChoice > mapCount-1 || playerChoice < 1) { // mapCount is +1 over the desired value, so mapCount-1
        std::cout << "Map file does not exist." << std::endl;
        mapFile.close();
        return;
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

            //Pass file path to mapToVector() in mapConvert.h to convert map file to usable vector
            mapToVector(mapFile);

            mapFile.close();
            return;
        }
        else {
            std::cout << "Map file not supported." << std::endl;
            mapFile.close();
            return;
        }
    }

}

#endif