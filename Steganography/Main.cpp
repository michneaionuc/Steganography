#include "opencv2/opencv.hpp"
#include <iostream>
#include "FrameProcessing.h"
#include "VideoProcessing.h"
#include "Encryption.h"
#include <string.h>

using namespace std;
using namespace cv;

void saveVideo(vector<Mat> frames, double FPS, int width, int height);

int main() {

    vector<Mat> frames;
    vector<Mat> stegoFrames;

    extract_frames("test.mp4", frames);

    VideoCapture cap("test.mp4");
    Mat img = imread("img.jpg");

    double FPS = static_cast<int>(cap.get(CAP_PROP_FPS));

    // Get the width/height of the video frames
    int width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));

    //Convert RGB to YUV
    //Split frame into 3 channels (Y, U, V)
    //permute pixels in each channel
    char key[] = "password";
    for (int i = 0; i < 50; i++) {

        Mat yuvFrame = convertFrameFromRGBToYCrCb(frames[i]);

        vector<Mat> frameChannels = splitFrameChannels(yuvFrame);

        Mat permutedY = permute(frameChannels[0], key);
        Mat permutedU = permute(frameChannels[1], key);
        Mat permutedV = permute(frameChannels[2], key);

        Mat permutedYInverse = permuteInverse(permutedY, key);
        Mat permutedUInverse = permuteInverse(permutedU, key);
        Mat permutedVInverse = permuteInverse(permutedV, key);

        vector<Mat> channels = { permutedYInverse, permutedUInverse, permutedVInverse };

        Mat mergedChannels = mergeFrameChannels(channels);

        stegoFrames.push_back(convertFrameFromYCrCbToRGB(mergedChannels));
    }

    saveVideo(stegoFrames, FPS, width, height);

    return 0;
}


void saveVideo(vector<Mat> frames, double FPS, int width, int height) {

    // Open a video file for writing (the MP4V codec works on OS X and Windows)
    cv::VideoWriter out("output.mov", cv::VideoWriter::fourcc('m', 'p', '4', 'v'), FPS, cv::Size(width, height));
    if (!out.isOpened()) {
        std::cout << "Error! Unable to open video file for output." << std::endl;
        std::exit(-1);
    }

    for (Mat frame : frames) {

        out << frame;
        cout << ".";

        //cv::imshow("Camera feed", frame);

        // Stop the camera if the user presses the "ESC" key
        if (cv::waitKey(1000.0 / FPS) == 27) break;
    }
}