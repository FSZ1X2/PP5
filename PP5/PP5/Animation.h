#pragma once
#include"Struct.h"
class Joint;
class Animation
{
	//Joint* joint;
	float totaltime;
	float framerate;
	float framerate_inv;
	int totalKeyframes;
public:
	float currtime = 0.0f;
	std::vector<std::vector<KeyFrame>> keyframes;
	void initializeBinaryAnimation(const char * path);
	//void sentToJoint(int _key);
	//void Interpolate(float delta, PosList& list);

	int GetTotalKeyframes() { return totalKeyframes; };
};

