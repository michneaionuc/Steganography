#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;


void extract_frames(const string& videoFilePath, vector<Mat>& frames);

void saveVideo(char* location, vector<Mat> frames, double FPS, int width, int height);