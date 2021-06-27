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
using namespace boost::multiprecision;

int main() {
    char fileLocation[] = "file.txt";
    unsigned char * secretMEssage = getSecretMessageInBytes(fileLocation);
    long length = strlen((char*)secretMEssage);

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

    vector<Mat> frames;
    vector<Mat> stegoFrames;
    vector<vector<int>> messageBits;
    extract_frames("test.mp4", frames);

    getStegoFrames(frames, permutedBits, stegoFrames, key);

    getMessageFromStegoFrames(stegoFrames, messageBits, permutedBits.size(), key);

    cout << "secret message = " << arraysOfBitsToSecretMessage(permuteMessageBitsInverse(messageBits, key));

    VideoCapture cap("test.mp4");

    double FPS = static_cast<int>(cap.get(CAP_PROP_FPS));

    // Get the width/height of the video frames
    int width = stegoFrames[0].cols;
    int height = stegoFrames[0].rows;

    char videoLocation[] = "output.mov";

    saveVideo(videoLocation, stegoFrames, FPS, width, height);

    return 0;
}


