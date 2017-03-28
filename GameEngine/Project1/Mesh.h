#pragma once
#include <string>
#include <vector>
#include <glm.hpp>
#include <GL/glew.h>

#include "Shader.h"
#include "Transform.h"
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
	FPS_Camera* camera;
	void Render(Transform transform, glm::vec3 lightPos);
	Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures, Shader _shader, FPS_Camera* _camera);

	~Mesh();
};

