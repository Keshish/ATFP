#include <iostream>

#include "Display/Display.h"

int main() {


    auto display = &Display::display();
    display->run();
    return 0;
}
