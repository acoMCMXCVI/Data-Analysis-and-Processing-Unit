#include "CalcData.h"


CalcData::CalcData()
{
}

CalcData::~CalcData() {}

void CalcData::calibration(const full_object_detection& face)
{

//	EYEBROWN
	noseroot = face.part(29).y();

	r_eyebrow_out = face.part(18).y();
	r_eyebrow_in = face.part(21).y();

	l_eyebrow_out = face.part(26).y();
	l_eyebrow_in = face.part(22).y();


//	EYELID
	r_eyelid_t = eyeCalcY(37, 38, face);
	r_eyelid_d = eyeCalcY(41, 40, face);
	r_eyelid_c = eyeCalcY(36, 36, face);//39

	l_eyelid_t = eyeCalcY(43, 44, face);
	l_eyelid_d = eyeCalcY(47, 46, face);
	l_eyelid_c = eyeCalcY(45, 45, face);//42
	
	r_eyelid_t_dist = r_eyelid_c - r_eyelid_t ;
	r_eyelid_d_dist = r_eyelid_c - r_eyelid_d ;
				 
	l_eyelid_t_dist = l_eyelid_c - l_eyelid_t;
	l_eyelid_d_dist = l_eyelid_c - l_eyelid_d;


//	NOSE
	noseroot_d = face.part(33).y();

	r_nose = face.part(31).y();
	l_nose = face.part(35).y();


//	MOUTH
	lipCorner_root = eyeCalcY(62, 66, face);

	r_lipCorner = face.part(48).y();
	l_lipCorner = face.part(54).y();

	r_lipCprner_dist = lipCorner_root - r_lipCorner  ;
	l_lipCprner_dist = lipCorner_root - l_lipCorner  ;


//	JAW
	jaw = ( eyeCalcY(7, 8, face) + face.part(9).y() )/2;


//	Calibration DONE
	caliCheck = true;

}

TunnelData CalcData::calculateTunnelData(const full_object_detection & faceRealTime )
{

	TunnelData tunnelData;

//	EYEBROW
	// ToDo: probati bez shifta
	float shift_eyebrow = faceRealTime.part(29).y() - this->noseroot;

	tunnelData.r_eyebrow_out = (this->r_eyebrow_out - faceRealTime.part(18).y() + shift_eyebrow);
	tunnelData.r_eyebrow_in = (this->r_eyebrow_in - faceRealTime.part(21).y() + shift_eyebrow);

	tunnelData.l_eyebrow_out = (this->l_eyebrow_out - faceRealTime.part(26).y() + shift_eyebrow);
	tunnelData.l_eyebrow_in = (this->l_eyebrow_in - faceRealTime.part(22).y() + shift_eyebrow);


//	EYELID
	tunnelData.r_eyelid_top = eyeCalcY(36, 36, faceRealTime) - eyeCalcY(37, 38, faceRealTime) - this->r_eyelid_t_dist;
	tunnelData.r_eyelid_down = eyeCalcY(36, 36, faceRealTime) - eyeCalcY(41, 40, faceRealTime) - this->r_eyelid_d_dist;

	tunnelData.l_eyelid_top = eyeCalcY(45, 45, faceRealTime) - eyeCalcY(43, 44, faceRealTime) - this->l_eyelid_t_dist;
	tunnelData.l_eyelid_down = eyeCalcY(45, 45, faceRealTime) - eyeCalcY(47, 46, faceRealTime) - this->l_eyelid_d_dist;


//	NOSE
	float shift_nose = faceRealTime.part(33).y() - this->noseroot_d;

	tunnelData.r_nose = this->r_nose - faceRealTime.part(31).y() + shift_nose;
	tunnelData.l_nose = this->l_nose - faceRealTime.part(35).y() + shift_nose;


//	MOUTH
	tunnelData.r_lipcorner = eyeCalcY(62, 66, faceRealTime) - faceRealTime.part(48).y() - this->r_lipCprner_dist;
	tunnelData.l_lipcorner = eyeCalcY(62, 66, faceRealTime) - faceRealTime.part(54).y() - this->l_lipCprner_dist;


//	JAW
	tunnelData.jaw =  this->jaw - (eyeCalcY(7, 8, faceRealTime) + faceRealTime.part(9).y()) / 2 ;





	cout << fixed << setprecision(3) << tunnelData.r_lipcorner << endl;
	return tunnelData;
}


float CalcData::eyeCalcY(int a, int b, const full_object_detection & faceRealTime)
{
	return (faceRealTime.part(a).y() + faceRealTime.part(b).y()) / 2;
}


bool CalcData::getcaliCheck()
{
	return caliCheck;
}


