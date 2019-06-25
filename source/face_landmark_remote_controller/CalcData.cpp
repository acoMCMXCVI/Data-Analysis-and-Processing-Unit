#include "CalcData.h"


CalcData::CalcData()
{
}

CalcData::CalcData(const full_object_detection& face){

//	EYEBROWN
	noseroot = face.part(29).y();
				 
	r_eyebrow_out = face.part(18).y();
	r_eyebrow_in = face.part(21).y();
				 
	l_eyebrow_out = face.part(22).y();
	l_eyebrow_in = face.part(26).y();

//	EYELID
	r_eyelid_t = ( face.part(53).y() + face.part(54).y() )/2 ;
	r_eyelid_d = ( face.part(57).y() + face.part(56).y() )/2 ;
	r_eyelid_c = ( face.part(52).y() + face.part(55).y() )/2 ;
			   	 
	l_eyelid_t = ( face.part(59).y() + face.part(60).y() )/2 ;
	l_eyelid_d = ( face.part(63).y() + face.part(62).y() )/2 ;
	l_eyelid_c = ( face.part(58).y() + face.part(61).y() )/2 ;
	
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
	r_eyelid_t = (face.part(37).y() + face.part(38).y()) / 2;
	r_eyelid_d = (face.part(41).y() + face.part(48).y()) / 2;
	r_eyelid_c = (face.part(36).y() + face.part(39).y()) / 2;

	l_eyelid_t = (face.part(43).y() + face.part(44).y()) / 2;
	l_eyelid_d = (face.part(47).y() + face.part(46).y()) / 2;
	l_eyelid_c = (face.part(42).y() + face.part(45).y()) / 2;
	
}

TunnelData CalcData::getTunnelData(const full_object_detection & faceRealTime, CalcData caliData)
{

	TunnelData tunnelData;
	// ToDo: probati bez shifta
	float shift = faceRealTime.part(29).y() - caliData.noseroot;

	tunnelData.r_eyebrow_out = (caliData.r_eyebrow_out - faceRealTime.part(18).y() + shift);
	tunnelData.r_eyebrow_in = (caliData.r_eyebrow_in - faceRealTime.part(21).y() + shift);
	tunnelData.l_eyebrow_out = (caliData.l_eyebrow_out - faceRealTime.part(26).y() + shift);
	tunnelData.l_eyebrow_in = (caliData.l_eyebrow_in - faceRealTime.part(22).y() + shift);


	return tunnelData;
}


