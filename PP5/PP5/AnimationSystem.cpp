#include "AnimationSystem.h"
#include"Joint.h"


void AnimationSystem::BlendAnimation(float delta, Animation* next)
{
	Interpolate(delta, current);
	PosList currentPose = list;
	Interpolate(delta, next);

	float ratio = std::fmin(timer / blendtotal, 1);
	XMVECTOR trans, rot, scale;
	XMVECTOR trans1, rot1, scale1;
	XMMATRIX m0, m1;
	for (int i = 0; i < joint->numOfJoint; i++)
	{
		m0 = XMLoadFloat4x4(&currentPose.pose[i]);
		m1 = XMLoadFloat4x4(&list.pose[i]);

		XMMatrixDecompose(&scale, &rot, &trans, m0);
		XMMatrixDecompose(&scale1, &rot1, &trans1, m1);

		trans = XMVectorLerp(trans, trans1, ratio);
		rot = XMQuaternionNormalize(XMQuaternionSlerp(rot, rot1, ratio));
		scale = XMVectorLerp(scale, scale1, ratio);
		m0 = XMMatrixAffineTransformation(scale, XMQuaternionIdentity(), rot, trans);

		XMStoreFloat4x4(&list.pose[i], m0);
	}

	if (ratio >= 1)
	{
		timer = 0.0f;
		current->currtime = 0.0f;
		current = next;
		ifBlend = false;
	}
	else
		timer += delta;
}

void AnimationSystem::BlendAnimation2(float delta, Animation * next)
{
	Interpolate(delta, current);
	PosList currentPose = list;
	sentToJoint(0, next);

	float ratio = std::fmin(timer / blendtotal, 1);
	XMVECTOR trans, rot, scale;
	XMVECTOR trans1, rot1, scale1;
	XMMATRIX m0, m1;
	for (int i = 0; i < joint->numOfJoint; i++)
	{
		m0 = XMLoadFloat4x4(&currentPose.pose[i]);
		m1 = XMLoadFloat4x4(&list.pose[i]);

		XMMatrixDecompose(&scale, &rot, &trans, m0);
		XMMatrixDecompose(&scale1, &rot1, &trans1, m1);

		trans = XMVectorLerp(trans, trans1, ratio);
		rot = XMQuaternionNormalize(XMQuaternionSlerp(rot, rot1, ratio));
		scale = XMVectorLerp(scale, scale1, ratio);
		m0 = XMMatrixAffineTransformation(scale, XMQuaternionIdentity(), rot, trans);

		XMStoreFloat4x4(&list.pose[i], m0);
	}

	if (ratio >= 1)
	{
		timer = 0.0f;
		current->currtime = 0.0f;
		current = next;
		ifBlend = false;
	}
	else
		timer += delta;
}

void AnimationSystem::Interpolate(float delta, Animation* Clip)
{
	//PosList list;
	for (int i = 0; i < (int)Clip->keyframes.size(); i++)
	{
		XMVECTOR trans, rot, scale;
		XMVECTOR trans1, rot1, scale1;
		XMMATRIX m0, m1;
		int key = 0;
		m0 = XMLoadFloat4x4(&Clip->keyframes[i][key].pose);
		if (Clip->keyframes[i].size() > 1)
		{
			while (1)
			{
				if (key + 1 >= (int)Clip->keyframes[i].size())
				{
					key = 0;
					Clip->currtime = 0;
					break;
				}
				if (Clip->currtime >= Clip->keyframes[i][key].time && Clip->currtime < Clip->keyframes[i][key + 1].time)
				{
					break;
				}
				key++;
			}
			m0 = XMLoadFloat4x4(&Clip->keyframes[i][key].pose);
			m1 = XMLoadFloat4x4(&Clip->keyframes[i][key + 1].pose);
			float interval = Clip->keyframes[i][key + 1].time - Clip->keyframes[i][key].time;
			float ratio = (Clip->currtime - Clip->keyframes[i][key].time) / interval;
			XMMatrixDecompose(&scale, &rot, &trans, m0);
			XMMatrixDecompose(&scale1, &rot1, &trans1, m1);

			trans = XMVectorLerp(trans, trans1, ratio);
			rot = XMQuaternionNormalize(XMQuaternionSlerp(rot, rot1, ratio));
			scale = XMVectorLerp(scale, scale1, ratio);
			m0 = XMMatrixAffineTransformation(scale, XMQuaternionIdentity(), rot, trans);


		}
		XMStoreFloat4x4(&list.pose[i], XMLoadFloat4x4(&joint->BindList.pos[i])  * m0);
	}
	//joint->poselist = list;
	Clip->currtime += delta;
}

void AnimationSystem::sentToJoint(int _key, Animation* Clip)
{
	//PosList list;
	for (int i = 0; i < (int)Clip->keyframes.size(); i++)
	{
		XMMATRIX m0;
		m0 = XMLoadFloat4x4(&Clip->keyframes[i][_key].pose);
		XMStoreFloat4x4(&list.pose[i], XMLoadFloat4x4(&joint->BindList.pos[i])  * m0);
	}
	//joint->poselist = list;
}

void AnimationSystem::setJoint()
{
	joint->poselist = list;
}
