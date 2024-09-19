#include"Camera.h"

using namespace glm;
using namespace std;

Camera::Camera(int width, int height, vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	// Initializes matrices since otherwise they will be the null matrix
	mat4 view = mat4(1.0f);
	mat4 projection = mat4(1.0f);

	view = lookAt(Position, Position + Orientation, Up); // Makes camera look in the right direction from the right position
	projection = perspective(radians(FOVdeg), (float)width / height, nearPlane, farPlane); // Adds perspective to the scene

	cameraMatrix = projection * view; // Sets new camera matrix
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, value_ptr(cameraMatrix)); // Exports camera matrix
}

void Camera::Inputs(GLFWwindow* window)
{
    // WASD Move
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Position += speed * Orientation;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Position += speed * -normalize(cross(Orientation, Up)) * 0.01f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Position += speed * -Orientation;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Position += speed * normalize(cross(Orientation, Up)) * 0.01f;
    
    // Up/Down
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		Position += speed * Up;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		Position += speed * -Up;
    
    // Faster
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
		speed = 0.4f;
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		speed = 0.1f;

    // Mouse
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // Hides mouse cursor

		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		double mouseX, mouseY; // Stores the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY); // Fetches the coordinates of the cursor

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		vec3 newOrientation = rotate(Orientation, radians(-rotX), normalize(cross(Orientation, Up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(angle(newOrientation, Up) - radians(90.0f)) <= radians(85.0f))
			Orientation = newOrientation;

		Orientation = rotate(Orientation, radians(-rotY), Up); // Calculates upcoming vertical change in the Orientation

		glfwSetCursorPos(window, (width / 2), (height / 2)); // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Unhides cursor since camera is not looking around anymore
		firstClick = true;
	}
}