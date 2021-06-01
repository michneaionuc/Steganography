#pragma once

#include "opencv2/opencv.hpp"

using namespace cv;

Mat permuteFramePixels(Mat frame, char key[]);

Mat permuteFramePixelsInverse(Mat frame, char key[]);

vector<int> permuteMessageBits(vector<vector<int>> messageBits, char key[]);

vector<vector<int>> permuteMessageBitsInverse(vector<int> messageBits, char key[]);