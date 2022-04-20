//For converting map file to usable 2D vector

#ifndef MAPCONVERT_H
#define MAPCONVERT_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "gameSession.h"

void mapToVector(std::ifstream& mapFile) {

    //Variables 
    std::vector<std::vector<char>> map; //Contain rows of the map
    std::vector<char> row; //A row consist of chunks
    std::string tempChunk; //Tempory chunk, used for checking if the chunk is part of the map.
    char chunk; //A chunk
    int rowIndex; // y-axis
    int chunkIndex; // x-asxis
    
    rowIndex=0;

    //Skip "LSMap" and empty line in file. Both were used for format checking in mapSelection.h 
    getline(mapFile, tempChunk);
    //getline(mapFile, tempChunk);

    //Convert map file to 2D array 'map'
    while (getline(mapFile, tempChunk)) {
        for (chunkIndex=0; tempChunk[chunkIndex] != '\0'; chunkIndex++) {
            chunk = tempChunk[chunkIndex];
            row.push_back(chunk);
        }
        map.push_back(row);
        row.clear();
        rowIndex++;
    }   
    mapFile.close();

    //For Debug use. Print 2D vector 'map' to see if map successfully converted
    /* 
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            std::cout << map[y][x];
        }
        std::cout << "\n";
    }
    //*/

    //Pass 'map' to gameSession
    startSection(map);
}

#endif