#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glm/glm.hpp>
#include<glad/glad.h>
#include<vector>

using namespace glm;
using namespace std;

// Structure to standardize the vertices used in the meshes
struct Vertex
{
    vec3 position;
	vec3 normal;
	vec3 color;
	vec2 texUV;
};

class VBO
{
    public:
        GLuint ID; // Reference ID of the Vertex Buffer Object
        VBO(vector<Vertex>& vertices); // Constructor that generates a Vertex Buffer Object and links it to vertices

        void Bind();
        void Unbind();
        void Delete();
};

#endif