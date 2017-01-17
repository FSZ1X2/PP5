#pragma once
#include"Struct.h"
class Joint;
class Animation
{
	Joint* joint;
	std::vector<std::vector<KeyFrame>> keyframes;
	float totaltime;
	float framerate;
	float framerate_inv;
	float currtime = 0.0f;
public:
	//void initializeAnimation(FBXExportDATA * _fbxflie, Joint* _joint);
	void initializeBinaryAnimation(const char * path, Joint* _joint);
	void sentToJoint();
	void Interpolate(float delta);
};

