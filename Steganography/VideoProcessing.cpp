
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

void saveVideo(char * location, vector<Mat> frames, double FPS, int width, int height) {

	// Open a video file for writing (the MP4V codec works on OS X and Windows)
	cv::VideoWriter out(location, cv::VideoWriter::fourcc('m', 'p', '4', 'v'), FPS, cv::Size(width, height));
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