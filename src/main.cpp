#include <iostream>

#include "Display/Display.h"
#include "Data.h"

int main() {
    auto data = &Data::data();
    data->readCsv();

    auto display = &Display::display();
    display->run();
    return 0;
}
