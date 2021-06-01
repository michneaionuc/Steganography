#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

unsigned char * getSecretMessageInBytes(char* fileName);

vector<int> unsignedchar2bits(unsigned char c);

unsigned char bits2unsignedchar(vector<int> bits);

vector<vector<int>> secretMessageToArraysOfBits(unsigned char* secretMessageInBytes);

unsigned char* arraysOfBitsToSecretMessage(vector<vector<int>> secretMessageInBits);