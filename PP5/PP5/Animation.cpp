#include "Animation.h"
#include "Joint.h"

void Animation::initializeAnimation(FBXExportDATA * _fbxflie, Joint* _joint)
{
	joint = _joint;
	totaltime = _fbxflie->GetAnimationTime();
	framerate = _fbxflie->GetFrameRate();
	framerate_inv = _fbxflie->GetFrameRate_Inv();
	//KeyFrame newFrames;
	int num = _fbxflie->keys.size();
	for (int i = 0; i < num; i++)
	{
		std::vector<KeyFrame> frames;
		int numOfKeyFrames = _fbxflie->keys[i].size();
		for (int j = 0; j < numOfKeyFrames; j++)
		{
			KeyFrame newFrames;
			newFrames.pose = _fbxflie->keys[i][j];
			newFrames.time = _fbxflie->keytime[i][j];
			frames.push_back(newFrames);
		}
		keyframes.push_back(frames);
	}
}

void Animation::sentToJoint()
{
}

void Animation::Interpolate(float delta)
{
	PosList list;
	for (int i = 0; i < keyframes.size(); i++)
	{
		XMVECTOR trans, rot, scale;
		XMVECTOR trans1, rot1, scale1;
		XMMATRIX m0, m1;
		int key = 0;
		m0 = XMLoadFloat4x4(&keyframes[i][key].pose);
		if (keyframes[i].size() > 1)
		{
			while (1)
			{
				if (key + 1 >= keyframes[i].size())
				{
					key = 0;
					currtime = 0;
					break;
				}
				if (currtime >= keyframes[i][key].time && currtime < keyframes[i][key + 1].time)
				{
					break;
				}
				key++;
			}
			m0 = XMLoadFloat4x4(&keyframes[i][key].pose);
			m1 = XMLoadFloat4x4(&keyframes[i][key + 1].pose);
			float interval = keyframes[i][key + 1].time - keyframes[i][key].time;
			float ratio = (currtime - keyframes[i][key].time) / interval;
			XMMatrixDecompose(&scale, &rot, &trans, m0);
			XMMatrixDecompose(&scale1, &rot1, &trans1, m1);

			trans = XMVectorLerp(trans, trans1, ratio);
			rot = XMQuaternionNormalize(XMQuaternionSlerp(rot, rot1, ratio));
			scale = XMVectorLerp(scale, scale1, ratio);
			m0 = XMMatrixAffineTransformation(scale, XMQuaternionIdentity(), rot, trans);
		}
		XMStoreFloat4x4(&list.pose[i],m0);
	}
	joint->poselist = list;
	currtime += delta;
}
