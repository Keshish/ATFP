#include <iostream>

#include "Actuator.h"
#include "Data.h"
#include "Display/Display.h"

int main() {
    auto data = &Data::data();
    data->readCsv();

    auto display = &Display::display();
    display->run();
    return 0;
}
