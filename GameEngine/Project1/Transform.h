#pragma once
#include <glm.hpp>
class Transform
{
private:
	glm::mat4 transform;
	glm::mat4 translateMat;
	glm::mat4 rotateMat;
	glm::mat4 scaleMat;
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	Transform() : position(0.0f), rotation(0.0f), scale(1.0f), transform(1.0f), translateMat(1.0f), rotateMat(1.0f), scaleMat(1.0f) {};
	Transform(glm::vec3 _pos) : position(_pos), rotation(0.0f), scale(1.0f), transform(1.0f), translateMat(1.0f), rotateMat(1.0f), scaleMat(1.0f) {};
	~Transform();
	void Translate(glm::vec3 _stepPos);
	void Rotate(glm::vec3 _stepRot);
	void Scale(glm::vec3 _stepScale);
	void UpdateMatrix();
	glm::mat4 GetMatrix() { return transform; };
};

