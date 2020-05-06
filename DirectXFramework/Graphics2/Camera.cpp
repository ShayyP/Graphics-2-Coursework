#include "Camera.h"

Camera::Camera()
{
    _position = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
    _moveLeftRight = 0.0f;
    _moveForwardBack = 0.0f;
    _yaw = 0.0f;
    _pitch = 0.0f;
    _roll = 0.0f;

	_right = XMVECTOR();
	_forward = XMVECTOR();
	_up = XMVECTOR();
	_viewMatrix = XMFLOAT4X4();
}

Camera::~Camera()
{
}

void Camera::SetPitch(float pitch)
{
    _pitch += XMConvertToRadians(pitch);
}

void Camera::SetTotalPitch(float pitch)
{
	_pitch = XMConvertToRadians(pitch);
}

float Camera::GetPitch() const
{
	return XMConvertToDegrees(_pitch);
}

void Camera::SetYaw(float yaw)
{
    _yaw += XMConvertToRadians(yaw);
}

void Camera::SetTotalYaw(float yaw)
{
	_yaw = XMConvertToRadians(yaw);
}

float Camera::GetYaw() const
{
	return XMConvertToDegrees(_yaw);
}

void Camera::SetRoll(float roll)
{
    _roll += XMConvertToRadians(roll);
}

void Camera::SetTotalRoll(float roll)
{
	_roll = XMConvertToRadians(roll);
}

float Camera::GetRoll() const
{
	return XMConvertToDegrees(_roll);
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

XMVECTOR Camera::GetPosition(void)
{
    return XMLoadFloat4(&_position);
}

void Camera::SetPosition(float x, float y, float z)
{
    _position = XMFLOAT4(x, y, z, 0.0f);
}

void Camera::Update(void)
{
	XMVECTOR cameraPosition;
	XMVECTOR cameraTarget;

	// Yaw (rotation around the Y axis) will have an impact on the forward and right vectors
	XMMATRIX cameraRotationYaw = XMMatrixRotationAxis(_defaultUp, _yaw);
	_right = XMVector3TransformCoord(_defaultRight, cameraRotationYaw);
	_forward = XMVector3TransformCoord(_defaultForward, cameraRotationYaw);

	// Pitch (rotation around the X axis) impact the up and forward vectors
	XMMATRIX cameraRotationPitch = XMMatrixRotationAxis(_right, _pitch);
	_up = XMVector3TransformCoord(_defaultUp, cameraRotationPitch);
	_forward = XMVector3TransformCoord(_forward, cameraRotationPitch);

	// Roll (rotation around the Z axis) will impact the Up and Right vectors
	XMMATRIX cameraRotationRoll = XMMatrixRotationAxis(_forward, _roll);
	_up = XMVector3TransformCoord(_up, cameraRotationRoll);
	_right = XMVector3TransformCoord(_right, cameraRotationRoll);

	// Adjust the camera position by the appropriate amount forward/back and left/right
	cameraPosition = XMLoadFloat4(&_position) + _moveLeftRight * _right + _moveForwardBack * _forward;
	XMStoreFloat4(&_position, cameraPosition);

	// Reset the amount we are moving
	_moveLeftRight = 0.0f;
	_moveForwardBack = 0.0f;

	// Calculate a vector that tells us the direction the camera is looking in
	cameraTarget = cameraPosition + XMVector3Normalize(_forward);

	// and calculate our view matrix
	XMStoreFloat4x4(&_viewMatrix, XMMatrixLookAtLH(cameraPosition, cameraTarget, _up));
}

void Camera::Update(XMVECTOR position, float yaw, float pitch, float roll, float forward, float up)
{
	// Yaw (rotation around the Y axis) will have an impact on the forward and right vectors
	XMMATRIX cameraRotationYaw = XMMatrixRotationAxis(_defaultUp, XMConvertToRadians(yaw));
	_right = XMVector3TransformCoord(_defaultRight, cameraRotationYaw);
	_forward = XMVector3TransformCoord(_defaultForward, cameraRotationYaw);

	// Pitch (rotation around the X axis) impact the up and forward vectors
	XMMATRIX cameraRotationPitch = XMMatrixRotationAxis(_right, XMConvertToRadians(pitch));
	_up = XMVector3TransformCoord(_defaultUp, cameraRotationPitch);
	_forward = XMVector3TransformCoord(_forward, cameraRotationPitch);

	// Roll (rotation around the Z axis) will impact the Up and Right vectors
	XMMATRIX cameraRotationRoll = XMMatrixRotationAxis(_forward, XMConvertToRadians(roll));
	_up = XMVector3TransformCoord(_up, cameraRotationRoll);
	_right = XMVector3TransformCoord(_right, cameraRotationRoll);

	// Adjust the camera position by the appropriate amount
	XMVECTOR cameraPosition = position - (_forward * forward) - (_up * up);
	XMStoreFloat4(&_position, cameraPosition);

	// Calculate a vector that tells us the direction the camera is looking in
	XMVECTOR cameraTarget = cameraPosition + XMVector3Normalize(_forward);

	// and calculate our view matrix
	XMStoreFloat4x4(&_viewMatrix, XMMatrixLookAtLH(cameraPosition, cameraTarget, _up));
}
