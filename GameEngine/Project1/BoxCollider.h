#pragma once
#include "Collider.h"
#include "FPS_Camera.h"
#include "Shader.h"

#include <GL/glew.h>
#include <glm.hpp>

class Transform;
class BoxCollider : public Collider
{
public:
	GLuint modelHandle, viewHandle, projHandle;
	Shader shader;

	float minX, minY, minZ;
	float maxX, maxY, maxZ;
	glm::vec3 center;
	glm::vec3 scale;
	BoxCollider() : minX(0.0f), minY(0.0f), minZ(0.0f), maxX(0.0f), maxY(0.0f), maxZ(0.0f), center(0.0f), scale(1.0f) {};
	BoxCollider(float _minX, float _minY, float _minZ, float _maxX, float _maxY, float _maxZ, glm::vec3 _centre, glm::vec3 _scale) : minX(_minX), minY(_minY), minZ(_minZ), maxX(_maxX), maxY(_maxY), maxZ(_maxZ), center(_centre), scale(_scale) {};
	BoxCollider(glm::vec3 _centre, glm::vec3 _scale) : center(_centre), scale(_scale) { UpdatePosition(_centre); };//InitGL(); };
	~BoxCollider();

	void InitGL();
	void UpdateBounds(float _minX, float _minY, float _minZ, float _maxX, float _maxY, float _maxZ);
	void UpdatePosition(glm::vec3 _pos);
	bool IsIntersectingBox(const BoxCollider &otherBox);
	void Draw(Transform transform, FPS_Camera* cam);
	virtual bool IsColliding(Collider collider) override;
};

