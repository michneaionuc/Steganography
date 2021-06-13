#include "opencv2/opencv.hpp"
#include <iostream>
#include "FrameProcessing.h"
#include "VideoProcessing.h"
#include "Encryption.h"
#include "Steganography.h"
#include "Message.h"
#include <string.h>

using namespace std;
using namespace cv;

void saveVideo(vector<Mat> frames, double FPS, int width, int height);

int main() {

    char fileLocation[] = "file.txt";
    unsigned char * secretMEssage = getSecretMessageInBytes(fileLocation);
    long length = strlen((char*)secretMEssage);
    cout << "xxxxxxxxx " << secretMEssage;

    vector<vector<int>> bits = secretMessageToArraysOfBits(secretMEssage);

    for (int i = 0; i < strlen((char*)secretMEssage); i++) {
        cout << "\nbits[" << i << "]= ";
        for (int j = 0; j < 8; j++) {
            cout << bits[i][j];
        }
    }

    unsigned char* secretMessageFromBits = arraysOfBitsToSecretMessage(bits);
    cout << "\nsecret message = " << secretMessageFromBits << "\n";

    char key[] = "password";

    vector<int> permutedBits = permuteMessageBits(bits, key);

    /*cout << "\nPermutedMEssage = ";
    for (int i = 0; i < permutedBits.size(); i++) {
        cout << permutedBits[i];
    }

    vector<vector<int>> unPermutedBits = permuteMessageBitsInverse(permutedBits, key);

    cout << "\nunPermutedMEssage = ";
    for (int i = 0; i < unPermutedBits.size(); i++) {
        for (int j = 0; j < 8; j++) {
            cout << unPermutedBits[i][j];
        }
    }

    unsigned char* secretMessageFromBitsUnpermuted = arraysOfBitsToSecretMessage(unPermutedBits);
    cout << "\nsecret message unpermuted= " << secretMessageFromBitsUnpermuted << "\n";*/

    vector<Mat> frames;
    vector<Mat> stegoFrames;

    extract_frames("test.mp4", frames);

    getStegoFrames(frames, permutedBits, stegoFrames, key);

    VideoCapture cap("test.mp4");
    Mat img = imread("img.jpg");

    double FPS = static_cast<int>(cap.get(CAP_PROP_FPS));

    // Get the width/height of the video frames
    int width = stegoFrames[0].cols;//static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
    int height = stegoFrames[0].rows;//static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));

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