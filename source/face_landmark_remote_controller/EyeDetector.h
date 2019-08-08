
#ifndef EYE_DETECTOR_H
#define EYE_DETECTOR_H



#include <dlib/image_processing/frontal_face_detector.h>

#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/features2d.hpp>

using namespace cv;
using namespace dlib;
using namespace std;

class EyeDetector {
public:

	EyeDetector();
	~EyeDetector();

	std::vector<float> calibrateEye(Mat frame, const full_object_detection & face);
	std::vector<float> eyesTracking ( Mat frame, const full_object_detection & face );

private:

	SimpleBlobDetector::Params parmsForDetector;
	Ptr<SimpleBlobDetector> detectorEye;
	cv::Rect eyeL;
	Mat eyeLframe;
	std::vector<float> eyeRealTime{0,0};

};
#endif
