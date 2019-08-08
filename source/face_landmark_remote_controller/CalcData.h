
#ifndef CALC_DATA_H
#define CALC_DATA_H


#include "TunnelData.h"


#include <dlib/image_processing/frontal_face_detector.h>
#include <iostream>

using namespace dlib;
using namespace std;

class CalcData {
public:

	CalcData();
	~CalcData();
	void calibration(const full_object_detection& face);
	void calibrationEyes(std::vector<float> coord);
	void calibrationSensors(std::vector<float> orient);
	TunnelData calculateTunnelData(const full_object_detection& faceRealTime, std::vector<float> orient, std::vector<float> eyes );
	bool getcaliCheck();


	void tunnelDataTurnToArray(const TunnelData data, std::vector < float>& arrayOfPositions);
	TunnelData arrayTurnToTunnelData(std::vector < float> arrayOfPositions);
	void initializeOldPostions();
	void turnToMatrixOldPositions(const TunnelData data);
	TunnelData calcAverage3 ();

	void calculateVel();

private:

	bool caliCheck = false;
	std::vector < float > positionOld{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	std::vector < float > positionNew{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	std::vector < float > acceleration{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	std::vector <std::vector< float >>oldPositions;

	ofstream myfileCalcData;
	
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
	float lipCorner_rootUD;

	float r_lipCornerUD;
	float l_lipCornerUD;

	float r_lipCorner_distUD;
	float l_lipCorner_distUD;

	float r_lipCornerLR;
	float l_lipCornerLR;

	float lip_d_root;
	
	float r_lip_d_dist;
	float l_lip_d_dist;

	float r_lip_d;
	float l_lip_d;


//	JAW
	float jaw;


// NECK
	float orientX;
	float orientY;
	float orientZ;

// EYES
	float eyeX;
	float eyeY;


// METODE

	float distCalcY(int a, int b, const full_object_detection & faceRealTime); // Racuna srednje tacke ociju posto se sve tri tacke sastoje iz po 2 tacke

};

#endif