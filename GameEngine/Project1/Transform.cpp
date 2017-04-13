#include "Transform.h"
#include <gtc\matrix_transform.hpp>
#include <iostream>


Transform::~Transform()
{
}

void Transform::SetPosition(glm::vec3 _pos)
{
	position += _pos;
	translateMat = glm::translate(translateMat, _pos);

	UpdateMatrix();
}


// TODO: this is currently useless
void Transform::Translate(glm::vec3 _stepPos)
{
	position += _stepPos;
	UpdateMatrix();
}

void Transform::Rotate(glm::vec3 _stepRot)
{
	rotation = _stepRot;
	rotateMat = glm::rotate(rotateMat, _stepRot.x, glm::vec3(1.0f, 0.0f, 0.0f));
	rotateMat = glm::rotate(rotateMat, _stepRot.y, glm::vec3(0.0f, 1.0f, 0.0f));
	rotateMat = glm::rotate(rotateMat, _stepRot.z, glm::vec3(0.0f, 0.0f, 1.0f));
	UpdateMatrix();
}

void Transform::Scale(glm::vec3 _stepScale)
{
	scale = _stepScale;
	scaleMat = glm::scale(scaleMat, _stepScale);
	UpdateMatrix();
}

void Transform::UpdateMatrix()
{
	//translateMat = glm::translate(translateMat, position);
	transform = translateMat * rotateMat * scaleMat;
}
