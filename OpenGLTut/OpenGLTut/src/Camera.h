#pragma once
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

enum MovementDirection { FRONT, BACK, LEFT, RIGHT };

class Camera {
private:
	glm::vec3 _pos;
	glm::vec3 _up;
	glm::vec3 _front;
	float _cameraSpeed;

	float pitch;
	float yaw;
	
	float _fov;
public:
	Camera(float FOV = 45.0);
	~Camera();

	glm::mat4 getCameraMatrix() const;

	void move(MovementDirection direction, float deltaTime);

	void setFOV(float fov);
};
