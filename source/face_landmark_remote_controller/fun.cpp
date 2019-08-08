#include "fun.h"



void drawMarkers( Mat frame, const full_object_detection & face, int first, int last )
{

	for (int j = first; first < last; j++ )								// Crtanje odredjenog dela lica
	{
		circle(frame, Point(face.part(48 + j).x(), face.part(48 + j).y()), 3, Scalar(255, 255, 255), 1, 8, 0);
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

void exportVideo(char &boolExport, string &path )
{

	cout << "Do you want to export video? y | n " << endl;
	cin >> boolExport;

	if (boolExport == 'y') {

		ostringstream nameOfvideo;
		cout << " Enter a name for a video: ";
		cin >> path;
		nameOfvideo << "../../source/face_landmark_remote_controller/Data/" << path << ".avi";
		path = nameOfvideo.str();

	}


}
