#pragma once

#include "opencv2/opencv.hpp"

using namespace cv;

Mat permute(Mat frame, char key[]);

Mat permuteInverse(Mat frame, char key[]);