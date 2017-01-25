#pragma once
#include"Struct.h"
#include"Animation.h"
class Joint;
class AnimationSystem
{
public:
	Animation* current;
	Animation* Clip1;
	Animation* Clip2;
	Joint* joint;
	PosList list;
	float timer = 0;
	float blendtotal = 0.2f;
	bool ifBlend = false;

	void BlendAnimation(float delta, Animation* next);
	void BlendAnimation2(float delta, Animation* next);
	void Interpolate(float delta, Animation* Clip);
	void sentToJoint(int _key, Animation* Clip);
	void setJoint();
};

