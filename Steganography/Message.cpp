#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

unsigned char * getSecretMessageInBytes(char* fileName) {
	unsigned char * secretMessage = NULL;
	long size = 0;
	FILE* fp;
	fopen_s(&fp, fileName, "rb");

	if (fp == NULL)
	{
		perror("Error while opening the file %s.\n");
		exit(EXIT_FAILURE);
	}

	//move at the end of the file to get the number of bytes so that we now the size of the buffer
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);	

	//allocate a buffer of size bytes
	secretMessage = (unsigned char*)calloc(size, sizeof(unsigned char));

	//move back at beggining of the file to read its content into the buffer
	fseek(fp, 0, SEEK_SET);
	if (secretMessage  != NULL) {
		fread(secretMessage, size, 1, fp);
	} else {
		perror("Error while alocating the buffer for bytes!\n");
	}

	secretMessage[size] = '\0';

	fclose(fp);

	return secretMessage;
}

vector<int> unsignedchar2bits(unsigned char c) {
	int i;
	vector<int> bits;
	for (i = 0; i < 8; i++) {
		bits.push_back((c & (1 << i)) != 0);
	}
	reverse(bits.begin(), bits.end());
	return bits;
}

unsigned char bits2unsignedchar(vector<int> bits) {
	int i;
	unsigned char result = 0;

	for (i = 0; i < 8; ++i)
		result |= (bits[i] == 1) << (7 - i);

	return result;
}

vector<vector<int>> secretMessageToArraysOfBits(unsigned char* secretMessageInBytes) {
	long messageLength = strlen((char*)secretMessageInBytes);
	vector<vector<int>> messageInBits;
		
	for (long i = 0; i < messageLength; i++) {
		messageInBits.push_back(unsignedchar2bits(secretMessageInBytes[i]));
	}
	return messageInBits;
}

unsigned char * arraysOfBitsToSecretMessage(vector<vector<int>> secretMessageInBits) {
	long messageLength = secretMessageInBits.size();
	unsigned char * secretMesage = (unsigned char*)calloc(messageLength, sizeof(unsigned char));

	for (long i = 0; i < messageLength; i++) {
		secretMesage[i] = bits2unsignedchar(secretMessageInBits[i]);
		for (int j = 0; j < 8; j++) {
			cout << secretMessageInBits[i][j];
		}
	}
	secretMesage[messageLength] = '\0';
	return secretMesage;
}