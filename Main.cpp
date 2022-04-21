#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include "headers/setting/customKeybind.h"
#include "headers/play/mapSelection.h"

//----------------Main driver----------------
int main() {

    mapSelect();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    return 0;
}

//--------------Menu options----------------

//Play

//Settings

//Exit


