#include"Model.h"

using namespace glm;
using namespace std;

Model::Model(const char* file)
{
	// Make a JSON object
	string text = get_file_contents(file);
	JSON = json::parse(text);

	// Get the binary data
	Model::file = file;
	data = getData();

	traverseNode(0); // Traverse all nodes
}

void Model::Draw(Shader& shader, Camera& camera)
{
	// Go over all meshes and draw each one
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Mesh::Draw(shader, camera, matricesMeshes[i]);
}

void Model::loadMesh(unsigned int indMesh)
{
	// Get all accessor indices
	unsigned int posAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
	unsigned int normalAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
	unsigned int texAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
	unsigned int indAccInd = JSON["meshes"][indMesh]["primitives"][0]["indices"];

	// Use accessor indices to get all vertices components
	vector<float> posVec = getFloats(JSON["accessors"][posAccInd]);
	vector<vec3> positions = groupFloatsVec3(posVec);
	vector<float> normalVec = getFloats(JSON["accessors"][normalAccInd]);
	vector<vec3> normals = groupFloatsVec3(normalVec);
	vector<float> texVec = getFloats(JSON["accessors"][texAccInd]);
	vector<vec2> texUVs = groupFloatsVec2(texVec);

	// Combine all the vertex components and also get the indices and textures
	vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);
	vector<GLuint> indices = getIndices(JSON["accessors"][indAccInd]);
	vector<Texture> textures = getTextures();

	meshes.push_back(Mesh(vertices, indices, textures)); 	// Combine the vertices, indices, and textures into a mesh
}

void Model::traverseNode(unsigned int nextNode, mat4 matrix)
{
	json node = JSON["nodes"][nextNode]; // Current node

	// Get translation if it exists
	vec3 translation = vec3(0.0f, 0.0f, 0.0f);
	if (node.find("translation") != node.end())
	{
		float transValues[3];
		for (unsigned int i = 0; i < node["translation"].size(); i++)
			transValues[i] = (node["translation"][i]);
		translation = make_vec3(transValues);
	}

	// Get quaternion if it exists
	quat rotation = quat(1.0f, 0.0f, 0.0f, 0.0f);
	if (node.find("rotation") != node.end())
	{
		float rotValues[4] =
		{
			node["rotation"][3],
			node["rotation"][0],
			node["rotation"][1],
			node["rotation"][2]
		};
		rotation = make_quat(rotValues);
	}

	// Get scale if it exists
	vec3 scale = vec3(1.0f, 1.0f, 1.0f);
	if (node.find("scale") != node.end())
	{
		float scaleValues[3];
		for (unsigned int i = 0; i < node["scale"].size(); i++)
			scaleValues[i] = (node["scale"][i]);
		scale = make_vec3(scaleValues);
	}

	// Get matrix if it exists
	mat4 matNode = mat4(1.0f);
	if (node.find("matrix") != node.end())
	{
		float matValues[16];
		for (unsigned int i = 0; i < node["matrix"].size(); i++)
			matValues[i] = (node["matrix"][i]);
		matNode = make_mat4(matValues);
	}

	// Initialize matrices
	mat4 trans = mat4(1.0f);
	mat4 rot = mat4(1.0f);
	mat4 sca = mat4(1.0f);

	// Use translation, rotation, and scale to change the initialized matrices
	trans = translate(trans, translation);
	rot = mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	mat4 matNextNode = matrix * matNode * trans * rot * sca; // Multiply all matrices together

	// Check if the node contains a mesh and if it does load it
	if (node.find("mesh") != node.end())
	{
		translationsMeshes.push_back(translation);
		rotationsMeshes.push_back(rotation);
		scalesMeshes.push_back(scale);
		matricesMeshes.push_back(matNextNode);

		loadMesh(node["mesh"]);
	}

	// Check if the node has children, and if it does, apply this function to them with the matNextNode
	if (node.find("children") != node.end())
	{
		for (unsigned int i = 0; i < node["children"].size(); i++)
			traverseNode(node["children"][i], matNextNode);
	}
}

vector<unsigned char> Model::getData()
{
	// Create a place to store the raw text, and get the uri of the .bin file
	string bytesText;
	string uri = JSON["buffers"][0]["uri"];

	// Store raw text data into bytesText
	string fileStr = string(file);
	string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
	bytesText = get_file_contents((fileDirectory + uri).c_str());

	// Transform the raw text data into bytes and put them in a vector
	vector<unsigned char> data(bytesText.begin(), bytesText.end());
	return data;
}

vector<float> Model::getFloats(json accessor)
{
	vector<float> floatVec;

	// Get properties from the accessor
	unsigned int buffViewInd = accessor.value("bufferView", 1);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	string type = accessor["type"];

	// Get properties from the bufferView
	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	// Interpret the type and store it into numPerVert
	unsigned int numPerVert;
	if (type == "SCALAR")
        numPerVert = 1;
	else if (type == "VEC2")
        numPerVert = 2;
	else if (type == "VEC3")
        numPerVert = 3;
	else if (type == "VEC4")
        numPerVert = 4;
	else
        throw invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3, or VEC4)");

	// Go over all the bytes in the data at the correct place using the properties from above
	unsigned int beginningOfData = byteOffset + accByteOffset;
	unsigned int lengthOfData = count * 4 * numPerVert;
	for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i)
	{
		unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
		float value;
		memcpy(&value, bytes, sizeof(float));
		floatVec.push_back(value);
	}

	return floatVec;
}

vector<GLuint> Model::getIndices(json accessor)
{
	vector<GLuint> indices;

	// Get properties from the accessor
	unsigned int buffViewInd = accessor.value("bufferView", 0);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	unsigned int componentType = accessor["componentType"];

	// Get properties from the bufferView
	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	// Get indices with regards to their type: unsigned int, unsigned short, or short
	unsigned int beginningOfData = byteOffset + accByteOffset;
	if (componentType == 5125)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i)
		{
			unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
			unsigned int value;
			memcpy(&value, bytes, sizeof(unsigned int));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5123)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
		{
			unsigned char bytes[] = { data[i++], data[i++] };
			unsigned short value;
			memcpy(&value, bytes, sizeof(unsigned short));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5122)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
		{
			unsigned char bytes[] = { data[i++], data[i++] };
			short value;
			memcpy(&value, bytes, sizeof(short));
			indices.push_back((GLuint)value);
		}
	}

	return indices;
}

vector<Texture> Model::getTextures()
{
	vector<Texture> textures;

	string fileStr = string(file);
	string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	// Go over all images
	for (unsigned int i = 0; i < JSON["images"].size(); i++)
	{
		string texPath = JSON["images"][i]["uri"]; // uri of current texture

		// Check if the texture has already been loaded
		bool skip = false;
		for (unsigned int j = 0; j < loadedTexName.size(); j++)
		{
			if (loadedTexName[j] == texPath)
			{
				textures.push_back(loadedTex[j]);
				skip = true;
				break;
			}
		}

		// If the texture has been loaded, skip this
		if (!skip)
		{
			// Load diffuse texture
			if (texPath.find("baseColor") != string::npos)
			{
				Texture diffuse = Texture((fileDirectory + texPath).c_str(), "diffuse", loadedTex.size());
				textures.push_back(diffuse);
				loadedTex.push_back(diffuse);
				loadedTexName.push_back(texPath);
			}
			// Load specular texture
			else if (texPath.find("metallicRoughness") != string::npos)
			{
				Texture specular = Texture((fileDirectory + texPath).c_str(), "specular", loadedTex.size());
				textures.push_back(specular);
				loadedTex.push_back(specular);
				loadedTexName.push_back(texPath);
			}
		}
	}

	return textures;
}

vector<Vertex> Model::assembleVertices(vector<vec3> positions, vector<vec3> normals, vector<vec2> texUVs)
{
	vector<Vertex> vertices;
	for (int i = 0; i < positions.size(); i++)
		vertices.push_back(Vertex{positions[i], normals[i], vec3(1.0f, 1.0f, 1.0f), texUVs[i]});   
	return vertices;
}

vector<vec2> Model::groupFloatsVec2(vector<float> floatVec)
{
	vector<vec2> vectors;
	for (int i = 0; i < floatVec.size(); i)
	    vectors.push_back(vec2(floatVec[i++], floatVec[i++]));
	return vectors;
}

vector<vec3> Model::groupFloatsVec3(vector<float> floatVec)
{
	vector<vec3> vectors;
	for (int i = 0; i < floatVec.size(); i)
		vectors.push_back(vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
	return vectors;
}

vector<vec4> Model::groupFloatsVec4(vector<float> floatVec)
{
	vector<vec4> vectors;
	for (int i = 0; i < floatVec.size(); i)
		vectors.push_back(vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
	return vectors;
}