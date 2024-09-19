#version 330 core

layout (location = 0) in vec3 aPos;          // Positions or Coordinates
layout (location = 1) in vec3 aNormal;       // Normals (not necessarily normalized)
layout (location = 2) in vec3 aColor;        // Colors
layout (location = 3) in vec2 aTex;          // Textures

out vec3 crntPos;                            // Output the current position for the Fragment Shader
out vec3 Normal;                             // Output the normal for the Fragment Shader
out vec3 color;                              // Output color for fragment shader
out vec2 texCoord;                           // Output texture coordinates to fragment shader

// Imports the camera matrix
uniform mat4 camMatrix;

// Imports the transformation matrices
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

void main()
{
	crntPos = vec3(model * translation * -rotation * scale * vec4(aPos, 1.0f));		// Calculates current position

	Normal = aNormal;                                  								// Assigns the normal from the Vertex Data to "Normal"
	color = aColor;                                    								// Assigns the colors from the Vertex Data to "color"
	texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;                               		// Assigns the texture coordinates from the Vertex Data to "texCoord"

	gl_Position = camMatrix * vec4(crntPos, 1.0);      								// Outputs the positions/coordinates of all vertices
}


