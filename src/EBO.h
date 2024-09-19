#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<glad/glad.h>
#include<vector>

using namespace std;

class EBO
{
    public:
        GLuint ID; // ID reference of Elements Buffer Object
        EBO(vector<GLuint>& indices); // Constructor that generates a Elements Buffer Object and links it to indices

        void Bind();
        void Unbind();
        void Delete();
};

#endif