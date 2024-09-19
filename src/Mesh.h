#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<string>

#include"VAO.h"
#include"EBO.h"
#include"Camera.h"
#include"Texture.h"

using namespace glm;
using namespace std;

class Mesh
{
public:
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;
	
	VAO VAO; // Store VAO in public so it can be used in the Draw function

	Mesh(vector <Vertex>& vertices, vector <GLuint>& indices, vector <Texture>& textures); // Initializes the mesh

	// Draws the mesh
	void Draw
	(
		Shader& shader, 
		Camera& camera,
		mat4 matrix = mat4(1.0f),
		vec3 translation = vec3(0.0f, 0.0f, 0.0f),
		quat rotation = quat(1.0f, 0.0f, 0.0f, 0.0f),
		vec3 scale = vec3(1.0f, 1.0f, 1.0f)
	);
};

#endif