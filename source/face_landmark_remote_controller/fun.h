#ifndef FUN_H
#define FUN_H


#include "TunnelData.h"


#include <dlib/image_processing/frontal_face_detector.h>

#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/features2d.hpp>


using namespace cv;
using namespace dlib;
using namespace std;



void drawMarkers( Mat frame, const full_object_detection & face, int first, int last );
void eyesTracking ( Mat frame, const full_object_detection & face );
void calcFps ( time_t start, time_t end );


#endif
