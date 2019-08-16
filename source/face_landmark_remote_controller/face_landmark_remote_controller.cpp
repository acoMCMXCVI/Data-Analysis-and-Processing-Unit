/*//////////////////////////////////////

			INSTRUCTION FOR LIVE STREAM

			SERVER	-TRUE
			CAMERA	-1

			ROTATE FRAME	ROTATE_90_CLOCKWISE


//////////////////////////////////////*/

#include "TunnelClient.h"
#include "fun.h"
#include "CalcData.h"
#include "EyeDetector.h"



#define SERVER  true							// Need for sever

#define CAMERA -2								// -2, -1 for video -- 0 for PC camera -- 1 for live capture

#define ROTATEFRAME ROTATE_90_COUNTERCLOCKWISE


#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/image_transforms.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/opencv/cv_image.h>
#include <dlib/opencv.h>
#include <iostream>

#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"


using namespace cv;
using namespace dlib;
using namespace std;


// ----------------------------------------------------------------------------------------

#define DEBUG(var) do{ cout << #var << " = " << var << endl; }while(0)

int main(int argc, char** argv)
{


	cout << "Face Capture System by AR" << endl << endl;
	cout << "-- Lenght of stick is recommended to be 26.5cm --" << endl << endl;


#if SERVER
	TunnelClient tunnelClient;
#endif // SERVER


    try
    {

        if (argc == 1)
        {
            cout << "Program nije pravilno pozvan" << endl;
            return 0;
        }


        frontal_face_detector detector = get_frontal_face_detector();					// Pronalazi face na slici

        shape_predictor sp;																// Shape predictor - istrenirana baza lica za predikciju 
        deserialize(argv[1]) >> sp;														// u Shape predictor postavljamo nas prvi argument



#if CAMERA >= 0																			// Odredjivanje kamere
		VideoCapture cap(CAMERA);
#elif CAMERA == -1
		VideoCapture cap("../../source/face_landmark_remote_controller/Data/mile.mp4");
#elif CAMERA == -2
		VideoCapture cap("../../source/face_landmark_remote_controller/Data/ema4Expresion.avi"); // ema4Expresion
#endif //CAMERA || CLIP
				
		if (!cap.isOpened()) {															// Provera da li je kamera ukljucena
			cout << "No cam or video...\n";
			return 0;
		}


		char boolExport = 'n';
		string name;																	// Export videa y | n
		exportVideo( boolExport, name );

		VideoWriter videoTest;															// Pravi VideoWriter jer bez nje ne moze kasnije 
if (boolExport == 'y')																	// Ako se traxi export onda se definise gde i kako
		videoTest.open( name, CV_FOURCC('M', 'J', 'P', 'G'), 29.97, Size(cap.get(CAP_PROP_FRAME_HEIGHT), cap.get(CAP_PROP_FRAME_WIDTH)));
		


#if SERVER
		tunnelClient.connectWithUnreal();
#endif // SERVER

		image_window win;																// Prozor dlib
		namedWindow("Window", WINDOW_AUTOSIZE);											// Prozor cv



		ofstream myfile;																// Pravljenje fajla za upis podataka
		myfile.open("../../source/face_landmark_remote_controller/Data/out.txt");

		TunnelData tunnelData;															// Podaci za slanje Urealu
		CalcData caliData;																// Podaci za kalibraciju
		EyeDetector eyeDet;
		std::vector<dlib::rectangle> facePos;											//Pravi se niz rectangle-ova koji okruzuju lice std::vector<dlib::rectangle> facePos;	


		time_t start, end;																// Merac vremena




////////////////					FOR PETLJA				/////////////////



        for (int i=0;i<10000;i++){														// For pelja za video


			Mat frame;
			cap >> frame;

			if ( i == 15 ) time(&start);												// Pocetak merenja vremena


#if CAMERA >= -1
			rotate(frame, frame, ROTATEFRAME);
#endif // CAMERA


			array2d<bgr_pixel> img;														// cv_image<bgr_pixel> img(frame);
			assign_image(img, cv_image<bgr_pixel>(frame));								// Pretvara Mat u array2d
            //pyramid_up(img);															// Skalira sliku kako bi se male face pronasle


			win.clear_overlay();														// Brise prosli frame i stavlja novi
			win.set_image(img);


			if (i > 10 && facePos.empty())
				facePos = detector(img);												// U facePos se ubacuju rectangleovi sa svim facama koje su pronadjene u slici 


			if (!facePos.empty()) {														// Provera postoji li uopste lice u bazi

				full_object_detection shape = sp(img, facePos[0]);						// Dobijanje lica ( shape-a ) iz slike
				win.add_overlay(render_face_detections(shape));

				//drawMarkers( frame, shape, 48, 68 );


if (boolExport == 'y')
				videoTest << frame;														// Export videa



				if (i > 10 && !caliData.getcaliCheck()) {								// KALIBRACIJA 

					cout << endl << "Calibration..." << endl;
					caliData.calibration(shape);										// Kalibracija sa klasom CalcData
					caliData.calibrationSensors(tunnelClient.calibrationOrient());
					caliData.calibrationEyes(eyeDet.calibrateEye(frame,shape));
					cout << "Calibration is done! \n" << endl << endl;

				}//Kraj kalibracije prvih 10 frejmova



				if (caliData.getcaliCheck()) {											// RACUNANJE VREDNOSTI 

					tunnelData = caliData.calculateTunnelData(shape, tunnelClient.getSensorsData(), eyeDet.eyesTracking(frame, shape));
					myfile << tunnelData.l_eyebrow_out << endl;
					//cout << tunnelData.orientX << "   " << tunnelData.l_eyebrow_in << endl;

#if SERVER			
					tunnelClient.sendTunnelData(tunnelData);							// Slanje podataka
#endif // Server

				}																		//Ako je kalibracija odradjena mozemo da racunamo vrednosti na osnovu kalibracije


			}
			else
				//cout << " No face " << endl;																			// Kraj provere da li postoji faca u facePos


			if (i == 135)																//Racunanje FPSa
				calcFps( start, end );


			//cv::rectangle(frame, rect, Scalar(255,120,35), 5, 8, 0);
			//imshow("Window", frame);
			waitKey(30);

        }//For petlja


	cap.release();
	videoTest.release();


    }																					//Try petlja



    catch (exception& e)
    {
        cout << "\nexception thrown!" << endl;
        cout << e.what() << endl;
    }
}