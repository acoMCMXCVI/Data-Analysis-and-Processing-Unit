
#ifndef CALC_DATA_H
#define CALC_DATA_H

#include <dlib/image_processing/frontal_face_detector.h>
#include <iostream>

using namespace dlib;
using namespace std;

#include "TunnelData.h"

class CalcData {
public:

	CalcData();
	~CalcData();
	void calibration(const full_object_detection& face);
	TunnelData calculateTunnelData(const full_object_detection& faceRealTime );
	bool getcaliCheck();

private:

	bool caliCheck = false;

//	TRESHHOLDS
	float eyebrown_tresh = 16; 
	float eyelid_t_tresh = 16;
	float eyelid_d_tresh = 16;
	float nose_trehs = 8;
	
//	EYEBROWN 
	float noseroot;

	float r_eyebrow_out;
	float r_eyebrow_in;

	float l_eyebrow_out;
	float l_eyebrow_in;

// EYELID
	float r_eyelid_t;
	float r_eyelid_d;
	float r_eyelid_c;

	float l_eyelid_t;
	float l_eyelid_d;
	float l_eyelid_c;

	float r_eyelid_t_dist;
	float r_eyelid_d_dist;

	float l_eyelid_t_dist;
	float l_eyelid_d_dist;

//	NOSE
	float noseroot_d;

	float r_nose;
	float l_nose;


//	MOUTH 
	float center_mouth_position;


	// lipCorners
	float lipCorner_root;

	float r_lipCorner;
	float l_lipCorner;

	float r_lipCprner_dist;
	float l_lipCprner_dist;


//	JAW
	float jaw;


// METODE

	float eyeCalcY(int a, int b, const full_object_detection & faceRealTime); // Racuna srednje tacke ociju posto se sve tri tacke sastoje iz po 2 tacke

};

#endif