#pragma once
#include "AbstractCamera.h"

#include <GL/glew.h>

#include <vector>
class Skybox
{
public:
	GLuint textureID;
	Skybox(std::vector<char*> images);
	void Render(AbstractCamera* camera);
	~Skybox();
};

