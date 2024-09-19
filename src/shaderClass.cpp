#include"shaderClass.h"
#include<cstring>

using namespace std;

// Reads a text file and outputs a string with everything in the text file
string get_file_contents(const char* filename)
{
	ifstream in(filename, ios::binary);
	if (in)
	{
		string contents;
		in.seekg(0, ios::end);
		contents.resize(in.tellg());
		in.seekg(0, ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

// Constructor that build the Shader Program from 2 different shaders
Shader::Shader(const char* vertexFile, const char* fragmentFile)
{

	// Read vertexFile and fragmentFile and store the strings
	string vertexCode = get_file_contents(vertexFile);
	string fragmentCode = get_file_contents(fragmentFile);

	// Convert the shader source strings into character arrays
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // Create Vertex Shader Object and get its reference
	glShaderSource(vertexShader, 1, &vertexSource, NULL); // Attach Vertex Shader source to the Vertex Shader Object
	glCompileShader(vertexShader); // Compile the Vertex Shader into machine code
	compileErrors(vertexShader, "VERTEX"); // Checks if Shader compiled succesfully

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Create Fragment Shader Object and get its reference
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL); // Attach Fragment Shader source to the Fragment Shader Object
	glCompileShader(fragmentShader); // Compile the Vertex Shader into machine code
	compileErrors(fragmentShader, "FRAGMENT"); // Checks if Shader compiled succesfully

	ID = glCreateProgram(); // Create Shader Program Object and get its reference
	
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	
	glLinkProgram(ID); // Wrap-up/Link all the shaders together into the Shader Program
	compileErrors(ID, "PROGRAM"); // Checks if Shaders linked succesfully

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

void Shader::Activate()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}

// Checks if the different Shaders have compiled properly
void Shader::compileErrors(unsigned int shader, const char* type)
{
	GLint hasCompiled; // Stores status of compilation
	char infoLog[1024]; // Character array to store error message in
	
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << endl;
		}
	}
}