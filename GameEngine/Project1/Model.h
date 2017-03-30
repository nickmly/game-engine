#pragma once
// System
#include <vector>

// Dependencies
#include "GL\glew.h"

// Engine
#include "Mesh.h"
#include "Shader.h"
#include "Transform.h"
#include "Component.h"

// ASSIMP
#include "Importer.hpp"
#include "scene.h"
#include "postprocess.h"

class Vertex;
class FPS_Camera;

class Model : public Component
{
private:
	Shader shader;
	std::vector<Mesh> meshes;
	std::vector<Texture> texturesLoaded;
	std::string directory;
	FPS_Camera* camera;
public:
	Transform transform;

	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	GLint TextureFromFile(const char* path, std::string directory);

	void Render(glm::vec3 lightPos);
	virtual void Update(float deltaTime) override;
	Model() {};
	Model(char* path, Shader _shader, FPS_Camera* _cam);
	~Model();
};

