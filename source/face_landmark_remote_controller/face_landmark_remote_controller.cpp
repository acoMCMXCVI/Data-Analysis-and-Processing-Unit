 /*iBUG 300-W face landmark dataset
    https://ibug.doc.ic.ac.uk/resources/facial-point-annotations/  
*/

#undef UNICODE


#include <winsock2.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define SERVER  false							//Need for sever

#define CAMERA -1

#define IP_ADDR "127.0.0.1"
#define IP_PORT 27000

#define BUFLEN 512



#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/image_transforms.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/opencv/cv_image.h>
#include <iostream>

#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"




using namespace cv;


using namespace dlib;
using namespace std;

#include "TunnelClient.h"

// ----------------------------------------------------------------------------------------

#define DEBUG(var) do{ cout << #var << " = " << var << endl; }while(0)

int main(int argc, char** argv)
{
	

#if SERVER
	TunnelClient tunnelClient;
#endif // SERVER


	cout << "FACE LENDMARK REMOTE CONTROLLER" << endl;
	namedWindow("Prozor", WINDOW_AUTOSIZE);


    try
    {

        if (argc == 1)
        {
            cout << "Program nije pravilno pozvan" << endl;
            return 0;
        }


        frontal_face_detector detector = get_frontal_face_detector();	// Pronalazi face na slici

        shape_predictor sp;												// Shape predictor - istrenirana baza lica za predikciju 
        deserialize(argv[1]) >> sp;

		ofstream myfile;												// Pravljenje fajla za upis podataka
		myfile.open("../../source/face_landmark_remote_controller/out.txt");




#if CAMERA >= 0
		VideoCapture cap(CAMERA);
#elif CAMERA -1
		VideoCapture cap("../../source/face_landmark_remote_controller/klip.mp4");
#endif //CAMERA || CLIP
				


		if (!cap.isOpened()) {											// Provera da li je kamera ukljucena
			cout << "Nema kamere\n";
			return 0;
		}
		
		cout << cap.get(CAP_PROP_FPS) << endl;


		image_window win;												// Prozor

		TunnelData tunnelData;											// Podaci za slanje Urealu

		TunnelData calibration;											// Podaci za kalibraciju
		bool caliCheck = false;											// Kalibracija y || n
		bool faceCheck = false;											//Postojili faca y || n

		time_t start, end;												// Merac vremena

		std::vector<dlib::rectangle> facePos;							//Pravi se niz rectangle-ova koji okruzuju lice std::vector<dlib::rectangle> facePos;	
		Rect rect;

        for (int i=0;i<1500;i++){										// For pelja za video

			//array2d<bgr_pixel> img;
			Mat frame;

			cap>> frame;

			if ( i == 15 ) time(&start);									// Pocetak merenja vremena


#if CAMERA != 0
			rotate(frame, frame, ROTATE_90_COUNTERCLOCKWISE);
#endif // CAMERA


			cv_image<bgr_pixel> img(frame);
			//assign_image(img, cv_image<bgr_pixel>(frame));			// Pretvara Mat u array2d
            //pyramid_up(img);											// Skalira sliku kako bi se male face pronasle


			win.clear_overlay();										// Brise prosli frame i stavlja novi
			win.set_image(img);

			if (i > 10 && facePos.empty()) {

				facePos = detector(img);									// U facePos se ubacuju rectangleovi sa svim facama koje su pronadjene u slici 
				rect.x = facePos[0].left();
				rect.y = facePos[0].top();
				rect.width = facePos[0].right()- facePos[0].left();
				rect.height = facePos[0].bottom() - facePos[0].top();
			}


			if (!facePos.empty()) {											// Provera postoji li uopste lice u bazi

				full_object_detection shape = sp(img, facePos[0]);			// DObijanje lica ( shape-a ) iz slike
				win.add_overlay(render_face_detections(shape));

				if (i > 10 && !caliCheck) {								// KALIBRACIJA 

					calibration.noseroot = shape.part(29).y();
					
					calibration.r_eyebrow_out = shape.part(18).y();
					calibration.r_eyebrow_in = shape.part(21).y();
					calibration.l_eyebrow_in = shape.part(22).y();
					calibration.l_eyebrow_out = shape.part(26).y();

					caliCheck = true;
					cout << "Uspesno kalibrisan sistem" << endl;

				}//Kraj kalibracije prvih 10 frejmova



				if (caliCheck) {											// RACUNANJE VREDNOSTI 

					float shift = shape.part(29).y() - calibration.noseroot;

					tunnelData.r_eyebrow_out = (calibration.r_eyebrow_out - shape.part(18).y() + shift);
					tunnelData.r_eyebrow_in = (calibration.r_eyebrow_in - shape.part(21).y() + shift) ;
					tunnelData.l_eyebrow_in = (calibration.l_eyebrow_in - shape.part(22).y() + shift);
					tunnelData.l_eyebrow_out = (calibration.l_eyebrow_out - shape.part(2).y() + shift);

					DEBUG(shift);
					DEBUG(tunnelData.r_eyebrow_in);

					myfile << tunnelData.r_eyebrow_in << endl;				//Upis podataka u fajl

#if SERVER																//Slanje podataka
					tunnelClient.sendTunnelData(tunnelData);
#endif // Server

				}//Ako je kalibracija odradjena mozemo da racunamo vrednosti na osnovu kalibracije

			}// Kraj provere da li postoji faca u facePos

			if (i == 135) {												// Zavrsetak merenja vremena

				time(&end);
				double seconds = difftime(end, start);
				DEBUG(seconds);

				// Calculate frames per second
				int fps = 120 / seconds;
				DEBUG(fps);

				myfile << "Broj FPSa je: "<< fps << "+" << seconds << endl;

			}//Racunanje FPSa
			
			cv::rectangle(frame, rect, Scalar(255,120,35), 5, 8, 0);
			imshow("Prozor", frame);
			waitKey(30);

        }//For petlja



    }//Try petlja



    catch (exception& e)
    {
        cout << "\nexception thrown!" << endl;
        cout << e.what() << endl;
    }
}