#pragma once
#include "Object.h"
#include "csv.h"
#include "fstream"
#include "iostream"
#include "sstream"
#include "vector"

class Data {
 public:
    explicit Data(const Data&) = delete;

    Data& operator=(const Data&) = delete;

    explicit Data(Data&&) = delete;

    Data& operator=(Data&&) = delete;

    static Data& data() {
        static Data instance;
        return instance;
    }

    std::vector<std::vector<Object>> objects;
    std::vector<double> timestamps;
    std::vector<double> yaws;
    std::vector<double> speeds;

    void readCsv(const std::string& path = "../assets/data/DevelopmentData.csv") {
        std::ifstream file(path);

        if (!file.is_open()) {
            std::cerr << "Error opening file!" << std::endl;
            exit(1);
        }

        // Read each line from the CSV file
        std::string line{};
        std::string header_item{};

        std::getline(file, line);
        std::istringstream header_iss(line);
        while (std::getline(header_iss, header_item, ';')) {
//            std::cout << header_item << " ";
        }
        std::cout << "\n";

        while (std::getline(file, line)) {
            std::istringstream data_iss(line);

            int row_id{};
            Object obj1{};
            Object obj2{};
            Object obj3{};
            Object obj4{};
            double speed{};
            double yaw{};
            double timestamp{};

            char t;
            data_iss >> row_id >> t >> obj1.x >> t >> obj1.y >> t >> obj2.x >> t >> obj2.y >> t >> obj3.x >> t >>
                obj3.y >> t >> obj4.x >> t >> obj4.y >> t >> speed >> t >> obj1.vel_x >> t >> obj1.vel_y >> t >>
                obj2.vel_x >> t >> obj2.vel_y >> t >> obj3.vel_x >> t >> obj3.vel_y >> t >> obj4.vel_x >> t >>
                obj4.vel_y >> t >> yaw >> t >> timestamp;

//            std::cout.precision(12);
//            std::cout << row_id << " " << obj1.x << " " << obj1.y << " " << obj2.x << " " << obj2.y << " " << obj3.x
//                      << " " << obj3.y << " " << obj4.x << " " << obj4.y << " " << speed << " " << obj1.vel_x << " "
//                      << obj1.vel_y << " " << obj2.vel_x << " " << obj2.vel_y << " " << obj3.vel_x << " " << obj3.vel_y
//                      << " " << obj4.vel_x << " " << obj4.vel_y << " " << yaw << " " << timestamp << "\n";
//
//            std::cout << obj1 << obj2 << obj3 << obj4 << speed << " " << yaw << " " << timestamp;
        }
    }

 private:
    Data() = default;
    ~Data() = default;
};