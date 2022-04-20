//For testing various functions in this game.

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "../outsideHeaders/dirent.h"

void mapToVector(std::ifstream& mapFile) {

    std::vector<std::vector<char>> mapChunks;
    std::vector<char> mapBlocks;
    std::string tempChunk;
    char tempBlock;
    int chunkIndex;
    int blockIndex;
    
    chunkIndex=0;
    getline(mapFile, tempChunk);
    getline(mapFile, tempChunk);
    while (getline(mapFile, tempChunk)) {
        for (blockIndex=0; tempChunk[blockIndex] != '\0'; blockIndex++) {
            tempBlock = tempChunk[blockIndex];
            mapBlocks.push_back(tempBlock);
        }
        mapChunks.push_back(mapBlocks);
        mapBlocks.clear();
        chunkIndex++;
    }   
    mapFile.close();

    for (int y = 0; y < mapChunks.size(); y++) {
        for (int x = 0; x < mapChunks[y].size(); x++) {
            std::cout << mapChunks[y][x];
        }
        std::cout << "\n";
    }
}

void mapSelect() {

    // Variables
    struct dirent* entry;
    DIR* dir = opendir("../maps");
    int mapCount = -1; 
    std::string mapName = "NULL";
    std::vector<std::string> mapList;
    std::ifstream mapFile;

    // List map files in 'maps' folder
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

    // Player choose a map file
    int playerChoice = 0;

    std::cout << std::endl << "Select map: " << std::endl;
    std::cin >> playerChoice;

    // Opening map file
    if (playerChoice > mapCount-1 || playerChoice < 1) { // mapCount is +1 over the desired value, hence >= instead of > when comparing playerChoice with mapCount.
        std::cout << "Map file does not exist." << std::endl;
        mapFile.close();
        return;
    }
    else {
        mapFile.open("maps/"+mapList[playerChoice-1]);

        std::string formatString;
        std::getline(mapFile, formatString);

        if (formatString == "LSMap") {
            while (std::getline(mapFile, formatString)) {
                std::cout << formatString << std::endl;
            }

            mapFile.close();

            //Pass file path to mapConvert.h for further processing. 
            
        }
        else {
            std::cout << "Map file not supported." << std::endl;
            mapFile.close();
            return;
        }
    }

}

int main() {

    /*
    std::ifstream mapFile("../maps/map1.txt");

    mapToVector(mapFile);

    mapFile.close();
    */

    /*
    mapSelect();
    */
}

