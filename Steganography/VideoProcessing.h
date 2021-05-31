#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;


void extract_frames(const string& videoFilePath, vector<Mat>& frames);

void save_frames(vector<Mat>& frames, const string& outputDir);