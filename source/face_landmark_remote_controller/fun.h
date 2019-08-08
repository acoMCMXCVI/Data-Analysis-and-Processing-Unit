#ifndef FUN_H
#define FUN_H


#include <dlib/image_processing/frontal_face_detector.h>

#include "opencv2/imgproc.hpp"



using namespace cv;
using namespace dlib;
using namespace std;


void drawMarkers( Mat frame, const full_object_detection & face, int first, int last );
void calcFps ( time_t start, time_t end );
void exportVideo ( char &boolExport, string &path );


#endif
