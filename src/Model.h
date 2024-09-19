#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include<json/json.h>
#include"Mesh.h"

using json = nlohmann::json;
using namespace glm;
using namespace std;

class Model
{
    public:
        Model(const char* file); // Loads in a model from a file and stores tha information in 'data', 'JSON', and 'file'
        void Draw(Shader& shader, Camera& camera);

    private:
        // Variables for easy access
        const char* file;
        vector<unsigned char> data;
        json JSON;

        // All the meshes and transformations
        vector<Mesh> meshes;
        vector<vec3> translationsMeshes;
        vector<quat> rotationsMeshes;
        vector<vec3> scalesMeshes;
        vector<mat4> matricesMeshes;

        // Prevents textures from being loaded twice
        vector<string> loadedTexName;
        vector<Texture> loadedTex;

        void loadMesh(unsigned int indMesh); // Loads a single mesh by its index

        void traverseNode(unsigned int nextNode, mat4 matrix = mat4(1.0f)); // Traverses a node recursively, so it essentially traverses all connected nodes

        vector<unsigned char> getData(); // Gets the binary data from a file
        
        // Interprets the binary data into floats, indices, and textures
        vector<float> getFloats(json accessor);
        vector<GLuint> getIndices(json accessor);
        vector<Texture> getTextures();

        // Assembles all the floats into vertices
        vector<Vertex> assembleVertices
        (
            vector<vec3> positions, 
            vector<vec3> normals, 
            vector<vec2> texUVs
        );

        // Helps with the assembly from above by grouping floats
        vector<vec2> groupFloatsVec2(vector<float> floatVec);
        vector<vec3> groupFloatsVec3(vector<float> floatVec);
        vector<vec4> groupFloatsVec4(vector<float> floatVec);
};

#endif