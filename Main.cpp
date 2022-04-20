#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include "headers/customKeybind.h"
#include "headers/mapSelection.h"

//----------------Main driver----------------
int main() {

    mapSelect();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    return 0;5
}

//--------------Menu options----------------

//Play

//Settings

//Exit


