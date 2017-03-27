#pragma once
#include <string>
#include <vector>
#include <glm.hpp>

#include "Shader.h"
#include "GL\glew.h"
#include "scene.h"

class Vertex;

class FPS_Camera;



struct Texture
{
	GLuint id;
	std::string type;
	aiString path;
};

class Mesh
{
private:
	void SetupMesh();
	Shader shader;
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	GLuint VAO, VBO, EBO;
	GLuint modelHandle, viewHandle, projHandle;
	void Render(glm::mat4 transform, FPS_Camera* cam);
	Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures, Shader _shader);


	~Mesh();
};

