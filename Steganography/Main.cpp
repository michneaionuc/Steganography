#include "opencv2/opencv.hpp"
#include <iostream>
#include "FrameProcessing.h"
#include "VideoProcessing.h"
#include "Encryption.h"
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

    cout << "\nPermutedMEssage = ";
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
    cout << "\nsecret message unpermuted= " << secretMessageFromBitsUnpermuted << "\n";

    /*vector<Mat> frames;
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

        Mat permutedY = permuteFramePixels(frameChannels[0], key);
        Mat permutedU = permuteFramePixels(frameChannels[1], key);
        Mat permutedV = permuteFramePixels(frameChannels[2], key);

        Mat permutedYInverse = permuteFramePixelsInverse(permutedY, key);
        Mat permutedUInverse = permuteFramePixelsInverse(permutedU, key);
        Mat permutedVInverse = permuteFramePixelsInverse(permutedV, key);

        vector<Mat> channels = { permutedYInverse, permutedUInverse, permutedVInverse };

        Mat mergedChannels = mergeFrameChannels(channels);

        stegoFrames.push_back(convertFrameFromYCrCbToRGB(mergedChannels));
    }

    saveVideo(stegoFrames, FPS, width, height);*/

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