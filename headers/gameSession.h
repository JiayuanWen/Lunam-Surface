#ifndef GAMESESSION_H
#define GAMESESSION_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

//Map
std::vector<std::vector<char>> map;

//Player
int posX = 0; 
int posY = 0; 
int energy = 100; //Player energy from 100 to 0

//Signaling between files
bool ready = false;


//Import map from mapConvert.h , then place player at starting point.
void startSection(std::vector<std::vector<char>> map_import) {
    //Import map from mapConvert.h
    map = map_import;

    //Find '^' in map, that is the starting point.
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            //std::cout << map[y][x]; //For Debug, checking if map is still intact.
            if (map[y][x] == '^') {
                posX = x;
                posY = y;
                map[y][x] = 'T';
            }
        }
        //std::cout << "\n";
    }

    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            std::cout << map[y][x]; //For Debug, checking if map is still intact.
        }
        std::cout << "\n";
    }
}

//
void setPlayer(/*2-D vector of frame pass here*/ bool upY, bool downY, bool leftX, bool rightX) {
    
}

#endif