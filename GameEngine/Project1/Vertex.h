#pragma once
#include <glm.hpp>
class Vertex 
{
public:
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoord;
	glm::vec3 color;
	Vertex() : pos(0.0f), texCoord(0.0f), color(1.0f) {};
	~Vertex() {};
};