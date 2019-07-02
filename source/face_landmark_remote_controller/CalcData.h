
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
	TunnelData getTunnelData(const full_object_detection& faceRealTime );

private:

//	TRESHHOLDS
	float eyebrown_tresh = 16; 
	float eyelid_t_tre = 16;
	float eyelid_d_tre = 16;
	float nose_trehs = 8;
	
//	EYEBROWN 
	int noseroot;

	int r_eyebrow_out;
	int r_eyebrow_in;

	int l_eyebrow_out;
	int l_eyebrow_in;

// EYELID
	float r_eyelid_t;
	float r_eyelid_d;
	float r_eyelid_c;

	int l_eyelid_t;
	int l_eyelid_d;
	int l_eyelid_c;

	float r_eyelid_t_dist;
	float r_eyelid_d_dist;

	float l_eyelid_t_dist;
	float l_eyelid_d_dist;

//	NOSE
	int noseroot_d;

	int r_nose;
	int l_nose;



// METODE
	//ToDo
	float eyeCalcY(int a, int b, const full_object_detection & faceRealTime); // Racuna srednje tacke ociju posto se sve tri tacke sastoje iz po 2 tacke

};

#endif