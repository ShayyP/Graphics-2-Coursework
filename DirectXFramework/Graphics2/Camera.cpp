#include "Camera.h"

XMVECTOR defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
XMVECTOR defaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
XMVECTOR defaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

Camera::Camera()
{
    _cameraPosition = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
    _moveLeftRight = 0.0f;
    _moveForwardBack = 0.0f;
    _cameraYaw = 0.0f;
    _cameraPitch = 0.0f;
    _cameraRoll = 0.0f;
}

Camera::~Camera()
{
}

void Camera::SetPitch(float pitch)
{
    _cameraPitch += XMConvertToRadians(pitch);
}

void Camera::SetTotalPitch(float pitch)
{
	_cameraPitch = XMConvertToRadians(pitch);
}

float Camera::GetPitch() const
{
	return XMConvertToDegrees(_cameraPitch);
}

void Camera::SetYaw(float yaw)
{
    _cameraYaw += XMConvertToRadians(yaw);
}

void Camera::SetTotalYaw(float yaw)
{
	_cameraYaw = XMConvertToRadians(yaw);
}

float Camera::GetYaw() const
{
	return XMConvertToDegrees(_cameraYaw);
}

void Camera::SetRoll(float roll)
{
    _cameraRoll += XMConvertToRadians(roll);
}

void Camera::SetTotalRoll(float roll)
{
	_cameraRoll = XMConvertToRadians(roll);
}

float Camera::GetRoll() const
{
	return XMConvertToDegrees(_cameraRoll);
}

void Camera::SetLeftRight(float leftRight)
{
    _moveLeftRight = leftRight;
}

void Camera::SetForwardBack(float forwardBack)
{
    _moveForwardBack = forwardBack;
}

XMMATRIX Camera::GetViewMatrix(void)
{
    return XMLoadFloat4x4(&_viewMatrix);
}

XMVECTOR Camera::GetCameraPosition(void)
{
    return XMLoadFloat4(&_cameraPosition);
}

void Camera::SetCameraPosition(float x, float y, float z)
{
    _cameraPosition = XMFLOAT4(x, y, z, 0.0f);
}

void Camera::Update(void)
{
	XMVECTOR cameraPosition;
	XMVECTOR cameraTarget;

	// Yaw (rotation around the Y axis) will have an impact on the forward and right vectors
	XMMATRIX cameraRotationYaw = XMMatrixRotationAxis(defaultUp, _cameraYaw);
	_cameraRight = XMVector3TransformCoord(defaultRight, cameraRotationYaw);
	_cameraForward = XMVector3TransformCoord(defaultForward, cameraRotationYaw);

	// Pitch (rotation around the X axis) impact the up and forward vectors
	XMMATRIX cameraRotationPitch = XMMatrixRotationAxis(_cameraRight, _cameraPitch);
	_cameraUp = XMVector3TransformCoord(defaultUp, cameraRotationPitch);
	_cameraForward = XMVector3TransformCoord(_cameraForward, cameraRotationPitch);

	// Roll (rotation around the Z axis) will impact the Up and Right vectors
	XMMATRIX cameraRotationRoll = XMMatrixRotationAxis(_cameraForward, _cameraRoll);
	_cameraUp = XMVector3TransformCoord(_cameraUp, cameraRotationRoll);
	_cameraRight = XMVector3TransformCoord(_cameraRight, cameraRotationRoll);

	// Adjust the camera position by the appropriate amount forward/back and left/right
	cameraPosition = XMLoadFloat4(&_cameraPosition) + _moveLeftRight * _cameraRight + _moveForwardBack * _cameraForward;
	XMStoreFloat4(&_cameraPosition, cameraPosition);

	// Reset the amount we are moving
	_moveLeftRight = 0.0f;
	_moveForwardBack = 0.0f;

	// Calculate a vector that tells us the direction the camera is looking in
	cameraTarget = cameraPosition + XMVector3Normalize(_cameraForward);

	// and calculate our view matrix
	XMStoreFloat4x4(&_viewMatrix, XMMatrixLookAtLH(cameraPosition, cameraTarget, _cameraUp));
}

void Camera::Update(XMVECTOR position, float yaw, float pitch, float roll, float forward, float up)
{
	// Yaw (rotation around the Y axis) will have an impact on the forward and right vectors
	XMMATRIX cameraRotationYaw = XMMatrixRotationAxis(defaultUp, XMConvertToRadians(yaw));
	_cameraRight = XMVector3TransformCoord(defaultRight, cameraRotationYaw);
	_cameraForward = XMVector3TransformCoord(defaultForward, cameraRotationYaw);

	// Pitch (rotation around the X axis) impact the up and forward vectors
	XMMATRIX cameraRotationPitch = XMMatrixRotationAxis(_cameraRight, XMConvertToRadians(pitch));
	_cameraUp = XMVector3TransformCoord(defaultUp, cameraRotationPitch);
	_cameraForward = XMVector3TransformCoord(_cameraForward, cameraRotationPitch);

	// Roll (rotation around the Z axis) will impact the Up and Right vectors
	XMMATRIX cameraRotationRoll = XMMatrixRotationAxis(_cameraForward, XMConvertToRadians(roll));
	_cameraUp = XMVector3TransformCoord(_cameraUp, cameraRotationRoll);
	_cameraRight = XMVector3TransformCoord(_cameraRight, cameraRotationRoll);

	// Adjust the camera position by the appropriate amount
	XMVECTOR cameraPosition = position - (_cameraForward * forward) - (_cameraUp * up);
	XMStoreFloat4(&_cameraPosition, cameraPosition);

	// Calculate a vector that tells us the direction the camera is looking in
	XMVECTOR cameraTarget = cameraPosition + XMVector3Normalize(_cameraForward);

	// and calculate our view matrix
	XMStoreFloat4x4(&_viewMatrix, XMMatrixLookAtLH(cameraPosition, cameraTarget, _cameraUp));
}
