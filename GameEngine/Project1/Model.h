#pragma once

// TODO: refactor these includes to find out which should be put into CPP file

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
class Light;

class Model : public Component
{
private:
	Shader shader;
	std::vector<Mesh> meshes;
	std::vector<Texture> texturesLoaded;
	std::string directory;
	FPS_Camera* camera;
public:
	Transform *transform;

	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	GLint TextureFromFile(const char* path, std::string directory);

	void Render(Light dirLight, std::vector<Light> lights);
	virtual void Update(float deltaTime) override;
	Model() {};
	Model(char* path, Shader _shader, FPS_Camera* _cam, Transform *_transform);
	~Model();
};

