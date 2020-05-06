#pragma once
#include "core.h"
#include "DirectXCore.h"

// Base camera class, MoveableNode inherits from this
class Camera
{
public:
    Camera();
    ~Camera();

    virtual void Update();
    void Update(XMVECTOR position, float yaw, float pitch, float roll, float forward, float up);
    XMMATRIX GetViewMatrix();
    XMVECTOR GetPosition();
    inline XMVECTOR GetForwardVector(){ return _forward; }
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

protected:
    XMFLOAT4   _position;

    XMFLOAT4X4 _viewMatrix;

    float      _moveLeftRight;
    float      _moveForwardBack;

    float      _yaw;
    float      _pitch;
    float      _roll;

    XMVECTOR   _right;
    XMVECTOR   _forward;
    XMVECTOR   _up;

    XMVECTOR   _defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    XMVECTOR   _defaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    XMVECTOR   _defaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
};

