#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

#define ERR_NO_NUM -1
#define ERR_NO_MEM -2

int myRandom(int size) {
    int i, n;
    static int numNums = 0;
    static int* numArr = NULL;

    // Initialize with a specific size.

    if (size >= 0) {
        if (numArr != NULL)
            free(numArr);
        if ((numArr = (int*)malloc(sizeof(int) * size)) == NULL)
            return ERR_NO_MEM;
        for (i = 0; i < size; i++)
            numArr[i] = i;
        numNums = size;
    }

    // Error if no numbers left in pool.

    if (numNums == 0)
        return ERR_NO_NUM;

    // Get random number from pool and remove it (rnd in this
    //   case returns a number between 0 and numNums-1 inclusive).

    n = rand() % numNums;
    i = numArr[n];
    numArr[n] = numArr[numNums - 1];
    numNums--;
    if (numNums == 0) {
        free(numArr);
        numArr = 0;
    }

    return i;
}

Mat permuteFramePixels(Mat frame, char key[]) {
    srand(atoi(key));
    Mat scrambledFrame = frame.clone();
    for (int i = 0; i < frame.rows; i++) {
        int j = 0;
        int randPosition = myRandom(frame.cols);
        int plm = 0;
        while (randPosition >= 0) {
            scrambledFrame.at<uchar>(i, j) = frame.at<uchar>(i, randPosition);
            j++;
            randPosition = myRandom(-1);
        }
    }

    return scrambledFrame;
}

Mat permuteFramePixelsInverse(Mat frame, char key[]) {
    srand(atoi(key));
    Mat scrambledFrame = frame.clone();
    for (int i = 0; i < frame.rows; i++) {
        int j = 0;
        int randPosition = myRandom(frame.cols);
        while (randPosition >= 0) {
            scrambledFrame.at<uchar>(i, randPosition) = frame.at<uchar>(i, j);
            j++;
            randPosition = myRandom(-1);
        }
    }

    return scrambledFrame;
}

vector<int> permuteMessageBits(vector<vector<int>> messageBits, char key[]) {
    srand(atoi(key));
    vector<int> permutedBitsMessage;
    //iterate over the vector of vector of bits
    for (int i = 0; i < messageBits.size(); i++) {
        //and for each byte, permute its pixels
        int permutedBits[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        int randPosition = myRandom(8); 
        int j = 0; //iterator over the vector of bits
        while (randPosition >= 0) {
            permutedBits[randPosition] = messageBits[i][j++];
            randPosition = myRandom(-1);            
        } 
        for (int i = 0; i < 8; i++) {
            permutedBitsMessage.push_back(permutedBits[i]);
        }
    }

    return permutedBitsMessage;
}

vector<vector<int>> permuteMessageBitsInverse(vector<int> messageBits, char key[]) {
    srand(atoi(key));
    vector<vector<int>> unPermutedBitsMessage;

    //iterate over the vector of bits
    for (int i = 0; i < messageBits.size(); i += 8) {
        vector<int> unPermutedBits = { 0, 0, 0, 0, 0, 0, 0, 0 };
        int randPosition = myRandom(8);
        int j = 0; //iterator over the vector of bits
        while (randPosition >= 0) {
            unPermutedBits.insert(unPermutedBits.begin() + j++, messageBits[i + randPosition]);            
            randPosition = myRandom(-1);            
        }
        vector<int> aux;
        for (auto it = unPermutedBits.begin(); it != unPermutedBits.end(); ++it) {
            aux.push_back(*it);            
        }
            
        unPermutedBitsMessage.push_back(aux);
    }

    return unPermutedBitsMessage;
}

