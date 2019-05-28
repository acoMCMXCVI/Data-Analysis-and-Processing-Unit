 /*iBUG 300-W face landmark dataset
    https://ibug.doc.ic.ac.uk/resources/facial-point-annotations/  
*/

#undef UNICODE


#include <winsock2.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

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
	TunnelClient tunnelClient;


	cout << "My BsC" << endl;
	namedWindow("Prozor", WINDOW_AUTOSIZE);
    try
    {

        if (argc == 1)
        {
            cout << "Call this program like this:" << endl;
            cout << "./face_landmark_detection_ex shape_predictor_68_face_landmarks.dat faces/*.jpg" << endl;
            cout << "\nYou can get the shape_predictor_68_face_landmarks.dat file from:\n";
            cout << "http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
            return 0;
        }



        frontal_face_detector detector = get_frontal_face_detector();	// Pronalazi face na slici

        shape_predictor sp;												// Shape predictor - istrenirana baza lica za predikciju 
        deserialize(argv[1]) >> sp;

		VideoCapture cap(0);											// Video

		if (!cap.isOpened()) {											// Provera da li je kamera ukljucena

			cout << "Nema kamere\n";
			return 0;

		}
		

		TunnelData tunnelData;											// Podaci za slanje Urealu

		image_window win;												// Prozor

		long eyebrow_height_min = LONG_MAX;
		long eyebrow_height_max = LONG_MIN;

		Point2f eyebrows_R_IN= Point2f(0,0) ;

        for (int i=0;i<100;i++){														// For pelja za video

            array2d<bgr_pixel> img;
			Mat frame;
			cap>> frame;

			assign_image(img, cv_image<bgr_pixel>(frame));				// Pretvara Mat u array2d

            //pyramid_up(img);											// Skalira sliku kako bi se male face pronasle


            std::vector<dlib::rectangle> dets = detector(img);			// U dets se ubacuju rectangleovi sa svim facama koje su pronadjene u slici 
			//std::vector<Point2f> points;

			win.clear_overlay();										// Brise prosli frame i stavlja novi
			win.set_image(img);



			if (!dets.empty()) {										// Ako postoji faca iscrtaj tu gacu preko postojeceg frejma
												
				full_object_detection shape = sp(img, dets[0]);
				win.add_overlay(render_face_detections(shape));


				if (i > 10 && eyebrows_R_IN == Point2f(0, 0) ) {        // KALIBRACIJA 

					point pointt;
					pointt = shape.part(17);

					eyebrows_R_IN.x = pointt.x();
					eyebrows_R_IN.y = pointt.y();

					cout << "Uspesno kalibrisan sistem" << endl;

				}

				if (i > 10 && eyebrows_R_IN != Point2f(0, 0)) {				// RACUNANJE VREDNOSTI 

					point pointt;
					pointt = shape.part(17);
					
					tunnelData.r_eyebrow_move = pointt.y() - eyebrows_R_IN.y;
					DEBUG(tunnelData.r_eyebrow_move);
					tunnelClient.sendTunnelData(tunnelData);

				}

				//DEBUG(eyebrow_height);

			}

			imshow("Prozor", frame);
			waitKey(30);
        }

    }

    catch (exception& e)
    {
        cout << "\nexception thrown!" << endl;
        cout << e.what() << endl;
    }
}