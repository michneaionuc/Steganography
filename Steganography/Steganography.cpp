#include <iostream>
#include "opencv2/opencv.hpp"
#include <string.h>
#include "boost/multiprecision/cpp_int.hpp"
#include "FrameProcessing.h"
#include "VideoProcessing.h"
#include "Message.h"
#include "Encryption.h"
using namespace boost::multiprecision;

using namespace std;
using namespace cv;

//returns 1 if yes, 0 otherwise
cpp_int getMaximumNumberOfBits(vector<Mat> inputFrames) {
	cpp_int numberOfFrames = inputFrames.size();
	cpp_int nrPixels = inputFrames[0].rows * inputFrames[0].cols;
	cpp_int maxNrOfMessageBitsInFrame = nrPixels * 3 + nrPixels * 2 * 2; //Y => 3 pixels, U => 2 pixels, V => 2 pixels
	cpp_int maxTotalNrOfMessageBits = numberOfFrames * maxNrOfMessageBitsInFrame;
	return maxTotalNrOfMessageBits; 
}



void getStegoFrames(vector<Mat> inputFrames, vector<int> inputBitsMessage, vector<Mat>& outputFrames, char * key) {
	cpp_int maxNrOfBits = getMaximumNumberOfBits(inputFrames);
	if (inputBitsMessage.size() <= maxNrOfBits) {
        int messageFinished = 0;
        //Convert RGB to YUV
        //permute pixels in each channel
        for (int i = 0; i < 50; i++) {
            vector<Mat> frameChannels = splitFrameChannels(inputFrames[i]);
            
            Mat permutedY = permuteFramePixels(frameChannels[0], key);
            Mat permutedU = permuteFramePixels(frameChannels[1], key);
            Mat permutedV = permuteFramePixels(frameChannels[2], key);
            
            for (int row = 0; row < permutedY.rows && messageFinished == 0; row++) {
                for (int col = 0; col < permutedY.cols && messageFinished == 0; col++) {
                    vector<int> currentYPixelValue = unsignedchar2bits(permutedY.at<uchar>(row, col)); 
                    vector<int> currentUPixelValue = unsignedchar2bits(permutedU.at<uchar>(row, col));
                    vector<int> currentVPixelValue = unsignedchar2bits(permutedV.at<uchar>(row, col));  

                    for (int bit = 5; bit < 8 && messageFinished == 0; bit++) {
                        if (inputBitsMessage.size() > 0) {
                            currentYPixelValue.at(bit) = inputBitsMessage.front();
                            inputBitsMessage.erase(inputBitsMessage.begin());                            
                        }
                        else {
                            messageFinished = 1;
                        }
                    }
                    for (int bit = 6; bit < 8 && messageFinished == 0; bit++) {
                        if (inputBitsMessage.size() > 0) {
                            currentUPixelValue.at(bit) = inputBitsMessage.front();
                            inputBitsMessage.erase(inputBitsMessage.begin());
                        }
                        else {
                            messageFinished = 1;
                        }
                    }
                    for (int bit = 6; bit < 8 && messageFinished == 0; bit++) {
                        if (inputBitsMessage.size() > 0) {
                            currentVPixelValue.at(bit) = inputBitsMessage.front();
                            inputBitsMessage.erase(inputBitsMessage.begin());
                        }
                        else {
                            messageFinished = 1;
                        }
                    }
                    unsigned char newYPixelValue = bits2unsignedchar(currentYPixelValue);
                    permutedY.at<uchar>(row, col) = newYPixelValue;

                    unsigned char newUPixelValue = bits2unsignedchar(currentUPixelValue);
                    permutedU.at<uchar>(row, col) = newUPixelValue;

                    unsigned char newVPixelValue = bits2unsignedchar(currentVPixelValue);
                    permutedV.at<uchar>(row, col) = newVPixelValue;
                }
            }

            Mat permutedYInverse = permuteFramePixelsInverse(permutedY, key);
            Mat permutedUInverse = permuteFramePixelsInverse(permutedU, key);
            Mat permutedVInverse = permuteFramePixelsInverse(permutedV, key);
            vector<Mat> channels = { permutedYInverse, permutedUInverse, permutedVInverse };
            Mat mergedChannels = mergeFrameChannels(channels);
            outputFrames.push_back(mergedChannels);
        }
	}
	else {
		cout << "\nThe file is to large! Select either a larger video or a smaller file!\n";
	}
}

void getMessageFromStegoFrames(vector<Mat> stegoFrames, vector<vector<int>>& bitsMessage, int messageSize, char* key) {
    vector<int> byte;
    int currNrBits = messageSize;
    //Convert RGB to YUV
    //Split frame into 3 channels (Y, U, V)
    //permute pixels in each channel
    for (int i = 0; i < 50 && currNrBits > 0; i++) {
        vector<Mat> frameChannels = splitFrameChannels(stegoFrames[i]);
        Mat permutedY = permuteFramePixels(frameChannels[0], key);
        Mat permutedU = permuteFramePixels(frameChannels[1], key);
        Mat permutedV = permuteFramePixels(frameChannels[2], key);

        for (int row = 0; row < permutedY.rows && currNrBits > 0; row++) {
            for (int col = 0; col < permutedY.cols && currNrBits > 0; col++) {
                vector<int> currentYPixelValue = unsignedchar2bits(permutedY.at<uchar>(row, col)); 
                vector<int> currentUPixelValue = unsignedchar2bits(permutedU.at<uchar>(row, col));
                vector<int> currentVPixelValue = unsignedchar2bits(permutedV.at<uchar>(row, col));

                for (int bit = 5; bit < 8 && currNrBits > 0; bit++) {                    
                    if (byte.size() == 8) {
                        bitsMessage.push_back(byte);
                        byte.clear();
                    }
                    byte.push_back(currentYPixelValue.at(bit));
                    currNrBits--;
                }

                for (int bit = 6; bit < 8 && currNrBits > 0; bit++) {
                    if (byte.size() == 8) {
                        bitsMessage.push_back(byte);
                        byte.clear();
                    }
                    byte.push_back(currentUPixelValue.at(bit));
                    currNrBits--;
                }
                for (int bit = 6; bit < 8 && currNrBits > 0; bit++) {
                    if (byte.size() == 8) {
                        bitsMessage.push_back(byte);
                        byte.clear();
                    }
                    byte.push_back(currentVPixelValue.at(bit));
                    currNrBits--;
                }
            }
        }
    }
}

