#include "MoveableNode.h"

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
	XMStoreFloat4x4(&_worldTransformation, XMLoadFloat4x4(&_defaultTransformation) * rotationYaw * rotationPitch * rotationRoll * XMMatrixTranslation(_position.x, _position.y, _position.z));
}