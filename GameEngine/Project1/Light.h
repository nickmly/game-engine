#pragma once
#include <glm.hpp>
class Light
{
public:
	glm::vec3 position, ambient, diffuse, specular;
	glm::vec4 direction = glm::vec4(0.0f);
	float linear, quadratic, constant;

	Light() : position(0.0f), ambient(1.0f), diffuse(1.0f), specular(1.0f), linear(0.0f), quadratic(0.0f), constant(0.0f) {};
	Light(glm::vec3 _pos, glm::vec3 _amb, glm::vec3 _diff, glm::vec3 _spec) : position(_pos), ambient(_amb), diffuse(_diff), specular(_spec), linear(0.0f), quadratic(0.0f), constant(0.0f) {};
	
	~Light();
};

