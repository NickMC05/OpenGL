#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

using namespace std;

// declare function for shader
string get_file_contents(const char* filename);

class Shader
{
    public:
        GLuint ID; // Reference ID of the Shader Program
        Shader(const char* vertexFile, const char* fragmentFile); // Constructor that build the Shader Program from 2 different shaders

        void Activate();
        void Delete();

    private:
        void compileErrors(unsigned int shader, const char* type); // Checks if the different Shaders have compiled properly
};

#endif