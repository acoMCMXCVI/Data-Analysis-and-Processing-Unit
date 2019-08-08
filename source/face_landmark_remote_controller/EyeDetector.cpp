#include "EyeDetector.h"


EyeDetector::EyeDetector()
{

	parmsForDetector.filterByConvexity = true;
	parmsForDetector.minConvexity = 0.05;

	detectorEye = SimpleBlobDetector::create(parmsForDetector);

}

EyeDetector::~EyeDetector() {}

std::vector<float> EyeDetector::calibrateEye(Mat frame, const full_object_detection & face)
{
	std::vector<float> eye;
	
	if (face.part(41).y() - face.part(37).y() > 10) {

		eyeL.x = face.part(37).x() - 20;
		eyeL.y = face.part(36).y() - 30;
		eyeL.height = face.part(41).y() - face.part(37).y() + 30;
		eyeL.width = face.part(39).x() - face.part(36).x() + 30;

		//circle(frame, Point(eyeL.x, eyeL.y), 3, Scalar(255, 255, 255), 1, 8, 0);


		eyeLframe = frame(eyeL);
		cvtColor(eyeLframe, eyeLframe, CV_BGR2GRAY);



		threshold(eyeLframe, eyeLframe, 100, 255, THRESH_BINARY);
		erode(eyeLframe, eyeLframe, Mat(), Point(-1, -1), 2);
		dilate(eyeLframe, eyeLframe, Mat(), Point(-1, -1), 4);
		medianBlur(eyeLframe, eyeLframe, 5);


		std::vector<KeyPoint> keypoints;
		detectorEye->detect(eyeLframe, keypoints);

		if (!keypoints.empty()) {

			eye.push_back(eyeL.y + keypoints[0].pt.y);
			eye.push_back(eyeL.x + keypoints[0].pt.x);

			eyeRealTime[0] = (eyeL.y + keypoints[0].pt.y);
			eyeRealTime[1] = (eyeL.x + keypoints[0].pt.x);


		}
		else {

			eye.push_back( (face.part(41).y() + face.part(37).y()) / 2 );
			eye.push_back( (face.part(39).x() + face.part(36).x()) / 2);
			eyeRealTime[0] = ((face.part(41).y() + face.part(37).y()) / 2);
			eyeRealTime[1] = ((face.part(39).x() + face.part(36).x()) / 2);

		}

	}
	else {

		eye.push_back((face.part(41).y() + face.part(37).y()) / 2);
		eye.push_back((face.part(39).x() + face.part(36).x()) / 2);
		eyeRealTime[0] = ((face.part(41).y() + face.part(37).y()) / 2);
		eyeRealTime[1] = ((face.part(39).x() + face.part(36).x()) / 2);

	}

	cout << eye[0] << "   " << eye[1] << endl;


	return eye;
}

std::vector<float> EyeDetector::eyesTracking( Mat frame, const full_object_detection & face )
{

	if (face.part(41).y() - face.part(37).y() > 10) {

		eyeL.x = face.part(37).x() - 20;
		eyeL.y = face.part(36).y() - 30;
		eyeL.height = face.part(41).y() - face.part(37).y() + 30;
		eyeL.width = face.part(39).x() - face.part(36).x() + 30;

		//circle(frame, Point(eyeL.x, eyeL.y), 3, Scalar(255, 255, 255), 1, 8, 0);


		eyeLframe = frame(eyeL);
		cvtColor(eyeLframe, eyeLframe, CV_BGR2GRAY);



		threshold(eyeLframe, eyeLframe, 100, 255, THRESH_BINARY);
		erode(eyeLframe, eyeLframe, Mat(), Point(-1, -1), 2);
		dilate(eyeLframe, eyeLframe, Mat(), Point(-1, -1), 4);
		medianBlur(eyeLframe, eyeLframe, 5);


		std::vector<KeyPoint> keypoints;
		detectorEye->detect(eyeLframe, keypoints);


		if (!keypoints.empty()) {

			eyeRealTime[0] = (eyeL.y + keypoints[0].pt.y);
			eyeRealTime[1] = (eyeL.x + keypoints[0].pt.x);

		}

		//drawKeypoints(eyeLframe, keypoints, eyeLframe, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
		//imshow("Window", eyeLframe);

	}

	//cout << eyeRealTime[0] << "   " << eyeRealTime[1] << endl;

	return eyeRealTime;

}
