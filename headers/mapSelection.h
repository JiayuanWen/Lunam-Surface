//This is where player select a map in 'maps' folder.

#ifndef MAPSELECTION_H
#define MAPSELECTION_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "../outsideHeaders/dirent.h"
#include "mapConvert.h"

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

    //Opening map file
    if (playerChoice > mapCount-1 || playerChoice < 1) { // mapCount is +1 over the desired value, hence >= instead of > when comparing playerChoice with mapCount.
        std::cout << "Map file does not exist." << std::endl;
        mapFile.close();
        return;
    }
    else {
        mapFile.open("maps/"+mapList[playerChoice-1]);

        std::string formatString;
        std::getline(mapFile, formatString);

        //Check map file format. All map files should begin with the string "LSMap" followed by one empty line
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