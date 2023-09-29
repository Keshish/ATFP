#include <iostream>

#include "Display/Display.h"

#include "opencv2/video/tracking.hpp"

using namespace cv;

int main() {
    KalmanFilter KF(2, 1, 0);

    auto display = &Display::display();
    display->run();
    return 0;
}
