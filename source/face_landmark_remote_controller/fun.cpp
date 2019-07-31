#include "fun.h"


void drawMarkers( Mat frame, const full_object_detection & face, int first, int last )
{

	for (int j = first; first < last; j++ )								// Crtanje odredjenog dela lica
	{
		circle(frame, Point(face.part(48 + j).x(), face.part(48 + j).y()), 3, Scalar(255, 255, 255), 1, 8, 0);
	}

}

void eyesTracking(Mat frame, const full_object_detection & face) {

	if ( face.part(41).y() - face.part(37).y() > 10 ){

		cv::Rect eyeL;
		eyeL.x = face.part(37).x() - 15;
		eyeL.y = face.part(36).y() - 10;
		eyeL.height = face.part(41).y() - face.part(37).y() + 5;
		eyeL.width = face.part(39).x() - face.part(36).x() + 5;

		//circle(frame, Point(eyeL.x, eyeL.y), 3, Scalar(255, 255, 255), 1, 8, 0);


		Mat eyeLframe = frame(eyeL);
		cvtColor(eyeLframe, eyeLframe, CV_BGR2GRAY);

		SimpleBlobDetector::Params parmsForDetector;


		// TODO nece da radiiiii ako je blob zalepljen za ivicuuu!!!!!!!!!!!!!!!


		parmsForDetector.filterByArea = true;
		parmsForDetector.minArea = 1500;

		//parmsForDetector.filterByCircularity = true;
		//parmsForDetector.minCircularity = 0.1;

		Ptr<SimpleBlobDetector> detectorEye = SimpleBlobDetector::create(parmsForDetector);
		std::vector<KeyPoint> keypoints;

		threshold(eyeLframe, eyeLframe, 60, 255, THRESH_BINARY );
		erode( eyeLframe, eyeLframe, Mat(), Point(-1,-1), 2 );
		dilate( eyeLframe, eyeLframe, Mat(), Point(-1, -1), 4 );
		medianBlur( eyeLframe, eyeLframe, 5 );



		detectorEye->detect(eyeLframe, keypoints);
		drawKeypoints(eyeLframe, keypoints, eyeLframe, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

		imshow("Window", eyeLframe);

	}

}

void calcFps( time_t start, time_t end)
{

		time(&end);
		double seconds = difftime(end, start);
		cout << "************************************" << seconds;

		// Calculate frames per second
		int fps = 120 / seconds;
		cout << fps << endl;


}
