#pragma once

#include "Data.h"

#include "opencv2/core.hpp"
#include "vector"
#include <cmath>

class Actuator {
 public:
    size_t run(const std::vector<std::vector<float>>& matVec) {
        cv::Mat mat(4, 2, CV_32F);
        for (int i = 0; i < mat.rows; i++) {
            for (int j = 0; j < mat.cols; j++) {
                mat.at<float>(i, j) = matVec[i][j];
            }
        }

        std::cout << mat << "\n";

//        double minDist, maxValue;
//        cv::Point minLoc, maxLoc;
//        cv::minMaxLoc(mat.col(0), &minDist, &maxValue, &minLoc, &maxLoc);
//
//        for (int i = 0; i < mat.rows; i++) {
//            mat.at<float>(i, 0) = minDist / mat.at<float>(i, 0);
//        }
//
//        double minValue, maxCollisionAngle;
//        minLoc, maxLoc;
//        cv::minMaxLoc(mat.col(1), &minDist, &maxValue, &minLoc, &maxLoc);
//
//        for (int i = 0; i < mat.rows; i++) {
//            if (maxValue != 0) {
//                mat.at<float>(i, 1) = mat.at<float>(i, 1) / maxValue;
//            } else {
//                mat.at<float>(i, 1) = 0;
//            }
//        }
//
//        cv::Mat weights(2, 1, CV_32F, {0.5, 0.5});
//
//        for (int i = 0; i < mat.rows; i++) {
//            mat.at<float>(i, 0) *= weights.at<float>(0);  // Multiply the first column
//            mat.at<float>(i, 1) *= weights.at<float>(1);  // Multiply the second column
//        }
//
//        cv::Mat rowSums;
//        cv::reduce(mat, rowSums, 1, cv::REDUCE_SUM);
//
//        std::cout << rowSums << "\n";
//
//        minValue, maxValue;
//        minLoc, maxLoc;
//        cv::minMaxLoc(rowSums, &minDist, &maxValue, &minLoc, &maxLoc);
//        return maxLoc.y;
        return 0;

    }
};