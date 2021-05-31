#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

vector<Mat> splitFrameChannels(Mat frame) {
    vector<Mat> Channels;
    split(frame, Channels);

    return Channels;
}

Mat mergeFrameChannels(vector<Mat> Channels) {
    Mat merged;
    merge(Channels, merged);
    return merged;
}

Mat convertFrameFromRGBToYCrCb(Mat frame) {
    Mat output(frame.size(), frame.type());
    cvtColor(frame, output, COLOR_BGR2YCrCb);
    return output;
}

Mat convertFrameFromYCrCbToRGB(Mat frame) {
    Mat output(frame.size(), frame.type());
    cvtColor(frame, output, COLOR_YCrCb2BGR);
    return output;
}
