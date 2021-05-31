
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

/*
This functions opens a video file and extracts the frames and put them into a vector of Mat(its the class for representing an img)
*/
void extract_frames(const string& videoFilePath, vector<Mat>& frames) {
	try {
		//open the video file
		VideoCapture cap(videoFilePath); // open the video file
		cap.set(CAP_PROP_FRAME_WIDTH, 320);//Setting the width of the video
		cap.set(CAP_PROP_FRAME_HEIGHT, 240);//Setting the height of the video//
		if (!cap.isOpened())  // check if we succeeded
			cout << "\nCan not open Video file \n" << videoFilePath;

		//cap.get(CV_CAP_PROP_FRAME_COUNT) contains the number of frames in the video;
		for (int frameNum = 0; frameNum < cap.get(CAP_PROP_FRAME_COUNT); frameNum++)
		{
			Mat frame;
			Mat resizedFrame;
			cap >> frame; // get the next frame from video
			resize(frame, resizedFrame, Size(100, 100), 0, 0, INTER_CUBIC); // resize to 320x240 resolution
			frames.push_back(frame);
		}
		cap.release();//Releasing the buffer memory//
	}
	catch (cv::Exception& e) {
		cerr << e.msg << endl;
		exit(1);
	}
}

/*
It saves a vector of frames into jpg images into the outputDir as 1.jpg,2.jpg etc where 1,2 etc represents the frame number
*/
void save_frames(vector<Mat>& frames, const string& outputDir) {
	vector<int> compression_params;
	compression_params.push_back(IMWRITE_JPEG_QUALITY);
	compression_params.push_back(100);

	long frameNumber = 0;

	for (std::vector<Mat>::iterator frame = frames.begin(); frame != frames.end(); ++frame) {
		string filePath = outputDir + to_string(static_cast<long long>(frameNumber)) + ".jpg";
		imwrite(filePath, *frame, compression_params);
	}


}