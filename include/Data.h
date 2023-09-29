#pragma once
#include "iostream"
#include "csv.h"
#include "Object.h"
#include "vector"

class Data{

public:
    explicit Data(const Data &) = delete;

    Data &operator=(const Data &) = delete;

    explicit Data(Data &&) = delete;

    Data &operator=(Data &&) = delete;

    static Data &data() {
        static Data instance;
        return instance;
    }

    std::vector<std::vector<Object>> objects;
    std::vector<double> timesamps;
    std::vector<double> yaws;

    static void readCsv(const std::string& path="../assets/data/DevelopmentData.csv"){
        io::CSVReader<19> in(path);

        in.read_header(io::ignore_extra_column,
                       "FirstObjectDistance_X", "FirstObjectDistance_Y",
                       "SecondObjectDistance_X", "SecondObjectDistance_Y",
                       "ThirdObjectDistance_X", "ThirdObjectDistance_Y",
                       "FourthObjectDistance_X", "FourthObjectDistance_Y",
                       "VehicleSpeed", "FirstObjectSpeed_X", "FirstObjectSpeed_Y",
                       "SecondObjectSpeed_X", "SecondObjectSpeed_Y",
                       "ThirdObjectSpeed_X", "ThirdObjectSpeed_Y",
                       "FourthObjectSpeed_X", "FourthObjectSpeed_Y",
                       "YawRate", "Timestamp");

    }

private:
    Data()=default;
    ~Data()=default;

};