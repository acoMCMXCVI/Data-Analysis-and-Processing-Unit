
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
	CalcData(const full_object_detection& face);
	~CalcData();
	void calibration(const full_object_detection& face);
	TunnelData getTunnelData(const full_object_detection& faceRealTime, CalcData);
private:
	
//	EYEBROWN 
	int noseroot;

	int r_eyebrow_out;
	int r_eyebrow_in;

	int l_eyebrow_out;
	int l_eyebrow_in;

// EYELID
	//int r_eyelid_top_l;
	//int r_eyelid_top_r;
	//int r_eyelid_down_l;
	//int r_eyelid_down_r;

	//int l_eyelid_top_l;
	//int l_eyelid_top_r;
	//int l_eyelid_down_l;
	//int l_eyelid_down_r;

	//int r_eyelid_center_r;
	//int r_eyelid_center_l;

	//int l_eyelid_center_r;
	//int l_eyelid_center_l;
	

	int r_eyelid_t;
	int r_eyelid_d;
	int r_eyelid_c;

	int l_eyelid_t;
	int l_eyelid_d;
	int l_eyelid_c;


};

#endif