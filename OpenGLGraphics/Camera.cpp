#include "Camera.h"
#include <GLFW/glfw3.h>

Camera::Camera(glm::vec3 postion, glm::vec3 up, float yaw, float pitch, float startMoveSpeed,float startTurnSpeed)
{
	this->position = postion;
	this->worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;
	this->front  = glm::vec3(0.0f, 0.0f, -1.0f);

	movementSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	Update(); // initialize other vectors

}
void Camera::Update() {
	//by default the front vector, so we need to update it
	// Update front vector, consider mapping to xz plane and use pitch first
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = -sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	// Normalize front vector
	front = glm::normalize(front);
	// Update right and up vectors
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));

}

/// <summary>
/// control camera with WASD
/// </summary>
/// <param name="keys"></param>
/// <param name="deltaTime"></param>
void Camera::KeyControl(bool* keys, GLfloat deltaTime) {
	GLfloat velocity = movementSpeed * deltaTime;
	if (keys[GLFW_KEY_W]) {
		position += front * velocity;
	}
	if (keys[GLFW_KEY_S]) {
		position -= front * velocity;
	}
	if (keys[GLFW_KEY_A]) {
		position -= right * velocity;
	}
	if (keys[GLFW_KEY_D]) {
		position += right * velocity;
	}
}

void Camera::MouseControl(GLfloat xChange, GLfloat yChange) {
	xChange *= turnSpeed;
	yChange *= turnSpeed;
	yaw -= xChange;
	pitch += yChange;
	// Limit pitch
	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	Update();
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(position, position + front, up);
}