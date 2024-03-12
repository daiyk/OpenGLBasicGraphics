#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GL/glew.h>
class Camera
{
public:
	Camera(glm::vec3 postion, glm::vec3 up, float yaw, float pitch, float startMoveSpeed, float startTurnSpeed);

	void KeyControl(bool* keys, GLfloat deltaTime);
	void MouseControl(GLfloat xChange, GLfloat yChange);
	glm::vec3 GetCameraPosition() { return position; }
	glm::mat4 getViewMatrix();
private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;
	GLfloat roll;

	GLfloat movementSpeed;
	GLfloat turnSpeed;

	void Update();
	
	
};

