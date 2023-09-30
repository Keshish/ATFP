#pragma once

#include "Data.h"

#include "vector"
#include <cmath>

class Actuator {
 public:
    size_t run(size_t tick) {
        auto data = &Data::data();

        std::vector<std::vector<float>> mat{};

        for (size_t i = 0; i < data->objects[tick].size(); i++) {
            auto& object = data->objects[tick][i];
            float dist = sqrt(pow(object.x, 2) + pow(object.y, 2));
            float collision_angle = 0;

            mat.push_back(std::vector<float>{dist, collision_angle});
        }

//        for (const auto& m : mat) {
//            for (const auto x : m) {
//                std::cout << x << " ";
//            }
//            std::cout << "\n";
//        }
//        std::cout << "\n";

        return 0;
    }
};