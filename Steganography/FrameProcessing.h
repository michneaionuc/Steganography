
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;


vector<Mat> splitFrameChannels(Mat frame);

Mat mergeFrameChannels(vector<Mat> Channels);