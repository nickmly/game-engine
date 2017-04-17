#pragma once
#include "AbstractCamera.h"
#include <vector>
class Skybox
{
public:
	Skybox(std::vector<char*> images);
	void Render(AbstractCamera* camera);
	~Skybox();
};

