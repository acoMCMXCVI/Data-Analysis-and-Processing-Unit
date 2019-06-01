 /*iBUG 300-W face landmark dataset
    https://ibug.doc.ic.ac.uk/resources/facial-point-annotations/  
*/

#undef UNICODE


#include <winsock2.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

//Need for sever
#define SERVER  0


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

		VideoCapture cap(1);											// Video

		if (!cap.isOpened()) {											// Provera da li je kamera ukljucena

			cout << "Nema kamere\n";
			return 0;

		}
		

		TunnelData tunnelData;											// Podaci za slanje Urealu

		image_window win;												// Prozor



		TunnelData calibration;
		bool caliCheck = false;

        for (int i=0;i<500;i++){										// For pelja za video

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


				if (i > 10 && !caliCheck) {			// KALIBRACIJA 
					
					calibration.r_eyebrow_out= shape.part(17).y();
					calibration.r_eyebrow_in = shape.part(21).y();
					calibration.l_eyebrow_in = shape.part(12).y();
					calibration.l_eyebrow_out = shape.part(26).y();

					caliCheck = true;
					cout << "Uspesno kalibrisan sistem" << endl;

				}

				if (caliCheck) {				// RACUNANJE VREDNOSTI 

					tunnelData.r_eyebrow_out = (shape.part(17).y()- calibration.r_eyebrow_out);
					tunnelData.r_eyebrow_in = (shape.part(21).y()- calibration.r_eyebrow_in) ;
					tunnelData.l_eyebrow_in = (shape.part(12).y()- calibration.l_eyebrow_in);
					tunnelData.l_eyebrow_out = (shape.part(26).y()- calibration.l_eyebrow_out);
					

					DEBUG(tunnelData.r_eyebrow_out);


#if SERVER																//Slanje podataka
					tunnelClient.sendTunnelData(tunnelData);
#endif // Server


				}

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