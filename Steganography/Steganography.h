#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void getStegoFrames(vector<Mat> inputFrames, vector<int> inputBitsMessage, vector<Mat>& outputFrames, char* key);