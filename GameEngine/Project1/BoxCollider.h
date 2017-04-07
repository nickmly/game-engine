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
	Transform* transform;
	GLuint modelHandle, viewHandle, projHandle;
	Shader shader;

	float minX, minY, minZ;
	float maxX, maxY, maxZ;
	glm::vec3 center;
	glm::vec3 scale;
	BoxCollider() : minX(0.0f), minY(0.0f), minZ(0.0f), maxX(0.0f), maxY(0.0f), maxZ(0.0f), center(0.0f), scale(1.0f) { name = "BoxCollider"; };

	BoxCollider(Transform* _transform);
	~BoxCollider();

	void InitGL();

	
	virtual void Update(float deltaTime) override;
	bool IsIntersectingBox(const BoxCollider &otherBox);
	void Draw(FPS_Camera* cam);
	virtual bool IsColliding(Collider collider) override;

	//void UpdateBounds(float _minX, float _minY, float _minZ, float _maxX, float _maxY, float _maxZ);
};

