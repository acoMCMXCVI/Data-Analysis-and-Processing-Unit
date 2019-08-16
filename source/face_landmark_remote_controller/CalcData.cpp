#include "CalcData.h"


CalcData::CalcData()
{
}

CalcData::~CalcData() {}

void CalcData::calibration(const full_object_detection& face)
{

	myfileCalcData.open("../../source/face_landmark_remote_controller/Data/out.tsv");
	initializeOldPostions();

//	EYEBROWN
	noseroot = face.part(29).y();

	r_eyebrow_out = face.part(18).y();
	r_eyebrow_in = face.part(21).y();

	l_eyebrow_out = face.part(26).y();
	l_eyebrow_in = face.part(22).y();


//	EYELID
	r_eyelid_t = distCalcY(37, 38, face);
	r_eyelid_d = distCalcY(41, 40, face);
	r_eyelid_c = distCalcY(36, 36, face);//39

	l_eyelid_t = distCalcY(43, 44, face);
	l_eyelid_d = distCalcY(47, 46, face);
	l_eyelid_c = distCalcY(45, 45, face);//42
	
	r_eyelid_t_dist = r_eyelid_c - r_eyelid_t ;
	r_eyelid_d_dist = r_eyelid_c - r_eyelid_d ;
				 
	l_eyelid_t_dist = l_eyelid_c - l_eyelid_t;
	l_eyelid_d_dist = l_eyelid_c - l_eyelid_d;


//	NOSE
	noseroot_d = face.part(33).y();

	r_nose = face.part(31).y();
	l_nose = face.part(35).y();


//	MOUTH
	lipCorner_rootUD = distCalcY(62, 66, face);

	r_lipCornerUD = face.part(48).y();
	l_lipCornerUD = face.part(54).y();

	r_lipCorner_distUD = lipCorner_rootUD - r_lipCornerUD;
	l_lipCorner_distUD = lipCorner_rootUD - l_lipCornerUD;

	r_lipCornerLR = face.part(48).x();
	l_lipCornerLR = face.part(54).x();

	lip_d_root = face.part(62).y();

	r_lip_d = face.part(59).y();
	l_lip_d = face.part(55).y();

	r_lip_d_dist = lip_d_root - r_lip_d;
	l_lip_d_dist = lip_d_root - l_lip_d;


//	JAW
	jaw = ( distCalcY(7, 8, face) + face.part(9).y() )/2;


//	Calibration DONE
	caliCheck = true;

}

void CalcData::calibrationEyes(std::vector<float> eye)
{

	eyeY = eye[0];
	eyeX = eye[1];

}

void CalcData::calibrationSensors(std::vector<float> orient)
{

	orientX = orient[0];
	orientY = orient[1];
	orientZ = orient[2];

}


TunnelData CalcData::calculateTunnelData(const full_object_detection & faceRealTime, std::vector<float> orient, std::vector<float> eyes )
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
	tunnelData.r_eyelid_top = distCalcY(36, 36, faceRealTime) - distCalcY(37, 38, faceRealTime) - this->r_eyelid_t_dist;
	tunnelData.r_eyelid_down = distCalcY(36, 36, faceRealTime) - distCalcY(41, 40, faceRealTime) - this->r_eyelid_d_dist;

	tunnelData.l_eyelid_top = distCalcY(45, 45, faceRealTime) - distCalcY(43, 44, faceRealTime) - this->l_eyelid_t_dist;
	tunnelData.l_eyelid_down = distCalcY(45, 45, faceRealTime) - distCalcY(47, 46, faceRealTime) - this->l_eyelid_d_dist;


//	NOSE
	float shift_nose = faceRealTime.part(33).y() - this->noseroot_d;

	tunnelData.r_nose = this->r_nose - faceRealTime.part(31).y() + shift_nose;
	tunnelData.l_nose = this->l_nose - faceRealTime.part(35).y() + shift_nose;

//	JAW
	float jaw_move = this->jaw - (distCalcY(7, 8, faceRealTime) + faceRealTime.part(9).y()) / 2;
	tunnelData.jaw = jaw_move;


//	MOUTH
	tunnelData.r_lipcornerUD = distCalcY(62, 66, faceRealTime) - faceRealTime.part(48).y() - this->r_lipCorner_distUD;
	tunnelData.l_lipcornerUD = distCalcY(62, 66, faceRealTime) - faceRealTime.part(54).y() - this->l_lipCorner_distUD;

	tunnelData.r_lipcornerLR = faceRealTime.part(48).x() - this->r_lipCornerLR;
	tunnelData.l_lipcornerLR = faceRealTime.part(54).x() - this->l_lipCornerLR;

	tunnelData.r_down_lip = faceRealTime.part(62).y() - faceRealTime.part(59).y() - this->r_lip_d_dist - jaw_move;
	tunnelData.l_down_lip = faceRealTime.part(62).y() - faceRealTime.part(55).y() - this->l_lip_d_dist - jaw_move;
	//cout << fixed << setprecision(3) << tunnelData.r_down_lip << endl;



//	NECK
	tunnelData.orientX = orientX - orient[0];
	tunnelData.orientY = orientY - orient[1];
	tunnelData.orientZ = orientZ - orient[2];

//	EYES
	cout << eyeY - eyes[0] << "   " << eyeX - eyes[1] << endl; 






	myfileCalcData << tunnelData.l_eyebrow_out << endl;


//	CONVERT TO MATRIX OF OLD POSITIONS FOR CALULATING AVERAGE
	turnToMatrixOldPositions(tunnelData);
	tunnelData = calcAverage3 ();





	//positionOld = positionNew;
	//tunnelDataTurnToArray(tunnelData, positionNew);
	//calculateVel();

	//cout << fixed << setprecision(3) << tunnelData.r_lipcornerUD << endl;
	return tunnelData;
}


float CalcData::distCalcY(int a, int b, const full_object_detection & faceRealTime)
{
	return (faceRealTime.part(a).y() + faceRealTime.part(b).y()) / 2;
}


bool CalcData::getcaliCheck()
{
	return caliCheck;
}

void CalcData::tunnelDataTurnToArray(const TunnelData data, std::vector < float>& arrayOfPositions )
{

	arrayOfPositions[0]=(data.r_eyebrow_out);
	arrayOfPositions[1]=(data.r_eyebrow_in);
	arrayOfPositions[2]=(data.l_eyebrow_out);
	arrayOfPositions[3]=(data.l_eyebrow_in);
					   
	arrayOfPositions[4]=(data.r_eyelid_top);
	arrayOfPositions[5]=(data.r_eyelid_down);
	arrayOfPositions[6]=(data.l_eyelid_top);
	arrayOfPositions[7]=(data.l_eyelid_down);
					   
	arrayOfPositions[8]=(data.r_nose);
	arrayOfPositions[9]=(data.l_nose);
					
	arrayOfPositions[10]=(data.r_lipcornerUD);
	arrayOfPositions[11]=(data.l_lipcornerUD);
	arrayOfPositions[12]=(data.r_lipcornerLR);
	arrayOfPositions[13]=(data.l_lipcornerLR);
	arrayOfPositions[14]=(data.r_down_lip);
	arrayOfPositions[15]=(data.l_down_lip);
						
	arrayOfPositions[16]=(data.jaw);
}

TunnelData CalcData::arrayTurnToTunnelData(std::vector<float> arrayOfPositions)
{
	TunnelData data;
	
	data.r_eyebrow_out = arrayOfPositions[0];
	data.r_eyebrow_in = arrayOfPositions[1];
	data.l_eyebrow_out= arrayOfPositions[2];
	data.l_eyebrow_in = arrayOfPositions[3];
					 
	data.r_eyelid_top = arrayOfPositions[4];
	data.r_eyelid_down = arrayOfPositions[5];
	data.l_eyelid_top = arrayOfPositions[6];
	data.l_eyelid_down = arrayOfPositions[7];
				
	data.r_nose	   = arrayOfPositions[8];
	data.l_nose	   = arrayOfPositions[9];
		
	data.r_lipcornerUD = arrayOfPositions[10];
	data.l_lipcornerUD = arrayOfPositions[11];
	data.r_lipcornerLR = arrayOfPositions[12];
	data.l_lipcornerLR = arrayOfPositions[13];
	data.r_down_lip   = arrayOfPositions[14];
	data.l_down_lip   = arrayOfPositions[15];
	
	data.jaw		   = arrayOfPositions[16];


	return data;
}

void CalcData:: calculateVel()
{

	for (int i = 0 ; i < positionNew.size() - 1; i++) {
		acceleration[i] = ( positionNew[i] - positionOld[i] ) / (1.0/30);
	}

	myfileCalcData << acceleration[2] << " \t " << positionNew[2] << " \t " << positionOld[2] << endl;
	//cout << acceleration[2] << endl;

}

void CalcData::initializeOldPostions()
{
	std::vector < float > zeros{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	oldPositions.push_back(zeros);
	oldPositions.push_back(zeros);
	oldPositions.push_back(zeros);
}

void CalcData::turnToMatrixOldPositions(const TunnelData data)
{

	std::vector < float > realTimePositions{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	tunnelDataTurnToArray(data, realTimePositions);


	oldPositions.erase(oldPositions.begin());
	oldPositions.push_back(realTimePositions);

	cout << oldPositions[0].size() << endl;

}

TunnelData CalcData::calcAverage3()
{

	TunnelData data;
	std::vector<float> average;

	int cols = oldPositions[0].size();

	for (int i = 0; i < cols; i++) {
		average.push_back((oldPositions[0][i] + oldPositions[1][i] + oldPositions[2][i])/3);
	}

	data = arrayTurnToTunnelData(average);

	return data;
}


