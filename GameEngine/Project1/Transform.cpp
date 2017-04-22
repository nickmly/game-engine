#include "Transform.h"
#include <gtc\matrix_transform.hpp>
#include <iostream>


Transform::~Transform()
{
}

void Transform::SetPosition(glm::vec3 _pos)
{	
	translateMat = glm::translate(translateMat, _pos - position);
	position = _pos;
	UpdateMatrix();
}


void Transform::Translate(glm::vec3 _stepPos)
{
	position += _stepPos;
	translateMat = glm::translate(translateMat, _stepPos);
	UpdateMatrix();
}

void Transform::SetRotationMatrix(glm::mat4 mat)
{
	rotateMat = mat;
	UpdateMatrix();
}

void Transform::SetRotation(glm::vec3 _rot)
{
	rotation = _rot;
	rotateMat = glm::rotate(rotateMat, _rot.x - rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	rotateMat = glm::rotate(rotateMat, _rot.y - rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	rotateMat = glm::rotate(rotateMat, _rot.z - rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	UpdateMatrix();
}

void Transform::Rotate(glm::vec3 _stepRot)
{
	rotation += _stepRot;
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
