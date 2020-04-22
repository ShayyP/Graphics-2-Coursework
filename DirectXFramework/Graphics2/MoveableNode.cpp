#include "MoveableNode.h"

void MoveableNode::SetPitch(float pitch)
{
    _pitch += XMConvertToRadians(pitch);
}

void MoveableNode::SetTotalPitch(float pitch)
{
    _pitch = XMConvertToRadians(pitch);
}

float MoveableNode::GetPitch() const
{
    return XMConvertToDegrees(_pitch);
}

void MoveableNode::SetYaw(float yaw)
{
    _yaw += XMConvertToRadians(yaw);
}

void MoveableNode::SetTotalYaw(float yaw)
{
    _yaw = XMConvertToRadians(yaw);
}

float MoveableNode::GetYaw() const
{
    return XMConvertToDegrees(_yaw);
}

void MoveableNode::SetRoll(float roll)
{
    _roll += XMConvertToRadians(roll);
}

void MoveableNode::SetTotalRoll(float roll)
{
    _roll = XMConvertToRadians(roll);
}

float MoveableNode::GetRoll() const
{
    return XMConvertToDegrees(_roll);
}

void MoveableNode::SetLeftRight(float leftRight)
{
    _moveLeftRight = leftRight;
}

void MoveableNode::SetForwardBack(float forwardBack)
{
    _moveForwardBack = forwardBack;
}

XMVECTOR MoveableNode::GetPosition(void)
{
    return XMLoadFloat4(&_position);
}

void MoveableNode::SetPosition(float x, float y, float z)
{
    _position = XMFLOAT4(x, y, z, 0.0f);
}

void MoveableNode::Update()
{
	XMVECTOR position;

	// Yaw (rotation around the Y axis) will have an impact on the forward and right vectors
	XMMATRIX rotationYaw = XMMatrixRotationAxis(_defaultUp, _yaw);
	_right = XMVector3TransformCoord(_defaultRight, rotationYaw);
	_forward = XMVector3TransformCoord(_defaultForward, rotationYaw);

	// Pitch (rotation around the X axis) impact the up and forward vectors
	XMMATRIX rotationPitch = XMMatrixRotationAxis(_right, _pitch);
	_up = XMVector3TransformCoord(_defaultUp, rotationPitch);
	_forward = XMVector3TransformCoord(_forward, rotationPitch);

	// Roll (rotation around the Z axis) will impact the Up and Right vectors
	XMMATRIX rotationRoll = XMMatrixRotationAxis(_forward, _roll);
	_up = XMVector3TransformCoord(_up, rotationRoll);
	_right = XMVector3TransformCoord(_right, rotationRoll);

	// Adjust the position by the appropriate amount forward/back and left/right
	position = XMLoadFloat4(&_position) + _moveLeftRight * _right + _moveForwardBack * _forward;
	XMStoreFloat4(&_position, position);

	// Reset the amount we are moving
	_moveLeftRight = 0.0f;
	_moveForwardBack = 0.0f;

	// Update the world transform matrix
	XMStoreFloat4x4(&_worldTransformation, XMMatrixRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), 90 * XM_PI / 180.0f) * XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), 180 * XM_PI / 180.0f) * rotationYaw * rotationPitch * rotationRoll * XMMatrixTranslation(_position.x, _position.y, _position.z));
}