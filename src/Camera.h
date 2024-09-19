#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H
#define GLM_ENABLE_EXPERIMENTAL

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"shaderClass.h"

using namespace glm;
using namespace std;

class Camera
{
    public:
	    // Stores the main vectors of the camera
        vec3 Position;
        vec3 Orientation = vec3(0.0f, 0.0f, -0.01f);
        vec3 Up = vec3(0.0f, 0.01f, 0.0f);
        mat4 cameraMatrix = mat4(1.0f);

        bool firstClick = true; // Prevents the camera from jumping around when first clicking left click
        
        int width, height; // Stores the width and height of the window

        // Adjust the speed of the camera and it's sensitivity when looking around
        float speed = 0.01f;
        float sensitivity = 100.0f;

        Camera(int width, int height, vec3 position); // Camera constructor to set up initial values

        void updateMatrix(float FOVdeg, float nearPlane, float farPlane); // Updates the camera matrix to the Vertex Shader
        void Matrix(Shader& shader, const char* uniform); // Exports the camera matrix to a shader
        
        void Inputs(GLFWwindow* window); // Handles camera inputs
};

#endif