#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	animationOn = true;
	printposition = false;

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	// calculate velocity based on deltaTime
	GLfloat velocity = moveSpeed * deltaTime;

	// looks into window.keys array and checks if certain key has been pressed 
	if (keys[GLFW_KEY_W])
	{
		position += front * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}

	if (keys[GLFW_KEY_SPACE])
	{
		position += up * velocity;
	}

	if (keys[GLFW_KEY_LEFT_CONTROL] || keys[GLFW_KEY_RIGHT_CONTROL])
	{
		position -= up * velocity;
	}

	if (keys[GLFW_KEY_F1])
	{
		animationOn = !animationOn;
	}

	if (keys[GLFW_KEY_P])
	{
		printposition = !printposition;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	//applying mouse movement to yaw and pitch for recalculation of camera vectors in update()
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}
glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

void Camera::update()
{
	//recalculate front vector after updating yaw and pitch
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	//calculate right and up vectors from the updated front vector
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::updatePosition(glm::vec3 nextPosition)
{
	position = nextPosition;

}

void Camera::updateOrientation(glm::vec3 tangent, glm::vec3 nextPoint)
{
	//// Calculate yaw angle based on the tangent vector
	//yaw = atan2(tangent.x, tangent.z);

	//// Calculate pitch angle based on the tangent vector
	//pitch = atan2(-tangent.y, glm::length(glm::vec2(tangent.x, tangent.z)));

	//look at the next bezier curve

	front = glm::normalize(tangent);
	right = glm::normalize(glm::cross(up, front));
	up = glm::cross(front, right);

	pitch = glm::degrees(asin(-front.y));
	yaw = glm::degrees(atan2(front.x, front.z));

	update();
	calculateViewMatrix();
}


Camera::~Camera()
{
}
