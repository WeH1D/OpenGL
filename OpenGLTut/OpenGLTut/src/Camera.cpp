#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>

Camera::Camera(float FOV) : _fov(FOV)
{
	_pos = glm::vec3(0.0, 0.0, 0.0);
	_front = glm::vec3(0.0, 0.0, -1.0);
	_up = glm::vec3(0.0, 1.0, 0.0);
	_cameraSpeed = 1.0f;
}

Camera::~Camera()
{
}

glm::mat4 Camera::getCameraMatrix() const
{
	glm::mat4 matrix = glm::lookAt(_pos, _pos + _front, _up);
	return matrix;
}

void Camera::move(MovementDirection direction, float deltaTime)
{
	switch (direction)
	{
	case FRONT:
		_pos += _cameraSpeed * _front * deltaTime;
		break;
	case BACK:
		_pos -= _cameraSpeed * _front * deltaTime;
		break;
	case LEFT:
		_pos += _cameraSpeed * glm::normalize(glm::cross(_up, _front)) * deltaTime;
		break;
	case RIGHT:
		_pos -= _cameraSpeed * glm::normalize(glm::cross(_up, _front)) * deltaTime;
		break;
	default:
		break;
	}
}

void Camera::setFOV(float fov)
{
	_fov = fov;
}
