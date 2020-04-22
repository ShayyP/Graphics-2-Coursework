#pragma once
#include "MeshNode.h"

class MoveableNode : public MeshNode
{
public:
	MoveableNode(wstring name, wstring modelName) : MeshNode(name, modelName)
	{
		_position = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		_moveLeftRight = 0.0f;
		_moveForwardBack = 0.0f;
		_yaw = 0.0f;
		_pitch = 0.0f;
		_roll = 0.0f;
		_right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
		_forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	}

	void Update();
	XMVECTOR GetPosition();
	inline XMVECTOR GetForwardVector() { return _forward; }
	inline XMVECTOR GetRightVector() { return _right; }
	inline XMVECTOR GetUpVector() { return _up; }
	void SetPosition(float x, float y, float z);
	void SetPitch(float pitch);
	void SetTotalPitch(float pitch);
	float GetPitch() const;
	void SetYaw(float yaw);
	void SetTotalYaw(float yaw);
	float GetYaw() const;
	void SetRoll(float roll);
	void SetTotalRoll(float roll);
	float GetRoll() const;
	void SetLeftRight(float leftRight);
	void SetForwardBack(float forwardBack);

private:
	XMFLOAT4 _position;

	float _yaw;
	float _pitch;
	float _roll;
	
	float _moveLeftRight;
	float _moveForwardBack;

	XMVECTOR _right;
	XMVECTOR _forward;
	XMVECTOR _up;

	XMVECTOR _defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR _defaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR _defaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
};

