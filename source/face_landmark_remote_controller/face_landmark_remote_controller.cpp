

#include "TunnelClient.h"
#include "fun.h"
#include "CalcData.h"



#define SERVER  true							// Need for sever

#define CAMERA -1								// -1 for video -- 0 for PC camera -- 1 for live capture


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


	cout << "Face Capture System by AR" << endl;


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
		VideoCapture cap("../../source/face_landmark_remote_controller/Data/ema.mp4");
#endif //CAMERA || CLIP
				
		if (!cap.isOpened()) {															// Provera da li je kamera ukljucena
			cout << "Nema kamere\n";
			return 0;
		}
		
		cout << cap.get(CAP_PROP_FPS) << endl;											// Ispis fps snimka




		char exportVideo = 'n';															// Export videa y | n
			cout << "Do you want to export video? y | n " << endl;
			cin >> exportVideo;

		VideoWriter videoTest;															// Pravi VideoWriter jer bez nje ne moze kasnije 
if (exportVideo == 'y')																	// Ako se traxi export onda se definise gde i kako
		videoTest.open("../../source/face_landmark_remote_controller/Data/outMileMouth.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10, Size(cap.get(CAP_PROP_FRAME_HEIGHT), cap.get(CAP_PROP_FRAME_WIDTH)));
		


#if SERVER
	//tunnelClient.connectWithUnreal();
#endif // SERVER

		image_window win;																// Prozor dlib
		namedWindow("Window", WINDOW_AUTOSIZE);											// Prozor cv



		ofstream myfile;																// Pravljenje fajla za upis podataka
		myfile.open("../../source/face_landmark_remote_controller/Data/out.txt");

		TunnelData tunnelData;															// Podaci za slanje Urealu

		CalcData caliData;																// Podaci za kalibraciju

		time_t start, end;																// Merac vremena

		std::vector<dlib::rectangle> facePos;											//Pravi se niz rectangle-ova koji okruzuju lice std::vector<dlib::rectangle> facePos;	



////////////////					FOR PETLJA				/////////////////



        for (int i=0;i<3000;i++){														// For pelja za video


			Mat frame;
			cap >> frame;

			if ( i == 15 ) time(&start);												// Pocetak merenja vremena


#if CAMERA != 0
			rotate(frame, frame, ROTATE_90_COUNTERCLOCKWISE);
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

				eyesTracking(frame, shape);
				//drawMarkers( frame, shape, 48, 68 );


if (exportVideo == 'y')
				videoTest << frame;														// Export videa



				if (i > 10 && !caliData.getcaliCheck()) {								// KALIBRACIJA 

					caliData.calibration(shape);										// Kalibracija sa klasom CalcData
					cout << "Uspesno kalibrisan sistem" << endl;

				}//Kraj kalibracije prvih 10 frejmova



				if (caliData.getcaliCheck()) {											// RACUNANJE VREDNOSTI 

					tunnelData = caliData.calculateTunnelData(shape);					// Umesto tunnelData moze i samo -- caliData.getTunnelData(shape, caliData); --
					myfile << tunnelData.l_eyelid_down << endl;

#if SERVER			
					tunnelClient.getSensorsData(tunnelData);
					//tunnelClient.sendTunnelData(tunnelData);							// Slanje podataka
#endif // Server

				}																		//Ako je kalibracija odradjena mozemo da racunamo vrednosti na osnovu kalibracije


			}																			// Kraj provere da li postoji faca u facePos


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