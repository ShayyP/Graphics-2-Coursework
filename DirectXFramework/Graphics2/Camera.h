#pragma once
#include "core.h"
#include "DirectXCore.h"

class Camera
{
public:
    Camera();
    ~Camera();

    void Update();
    void Update(XMVECTOR position, float yaw, float pitch, float roll, float forward, float up);
    XMMATRIX GetViewMatrix();
    XMVECTOR GetCameraPosition();
    inline XMVECTOR GetForwardVector(){ return _cameraForward; }
    inline XMVECTOR GetRightVector() { return _cameraRight; }
    inline XMVECTOR GetUpVector() { return _cameraUp; }
    void SetCameraPosition(float x, float y, float z);
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
    XMFLOAT4    _cameraPosition;

    XMFLOAT4X4  _viewMatrix;

    float       _moveLeftRight;
    float       _moveForwardBack;

    float       _cameraYaw;
    float       _cameraPitch;
    float       _cameraRoll;

    XMVECTOR _cameraRight;
    XMVECTOR _cameraForward;
    XMVECTOR _cameraUp;
};

