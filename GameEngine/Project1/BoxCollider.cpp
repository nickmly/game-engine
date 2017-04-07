#include "BoxCollider.h"
#include "FileReader.h"
#include "Transform.h"


#include <vector>

BoxCollider::BoxCollider(Transform * _transform)
{
	transform = _transform;
	name = "BoxCollider";
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::InitGL()
{
	modelHandle = glGetUniformLocation(shader.GetProgram(), "model"); // Assign modelHandle to uniform value model in shader program
	viewHandle = glGetUniformLocation(shader.GetProgram(), "view");
	projHandle = glGetUniformLocation(shader.GetProgram(), "projection");
}


void BoxCollider::Update(float deltaTime)
{
	center = transform->position;
	scale = transform->scale;

	minX = center.x - (scale.x/2);
	maxX = center.x + (scale.x/2);
							  
	minY = center.y - (scale.y/2);
	maxY = center.y + (scale.y/2);
							  
	minZ = center.z - (scale.z/2);
	maxZ = center.z + (scale.z/2);
}

bool BoxCollider::IsIntersectingBox(const BoxCollider &otherBox)
{
	if (minX <= otherBox.maxX && maxX >= otherBox.minX &&
		minY <= otherBox.maxY && maxY >= otherBox.minY &&
		minZ <= otherBox.maxZ && maxZ >= otherBox.minZ) {		
		return true;
	}
	return false;
}

void BoxCollider::Draw(FPS_Camera* cam)
{
	Shader shader = Shader(FileReader::ReadFromFile("Shaders/model_shader_basic.vert").c_str(), FileReader::ReadFromFile("Shaders/model_shader_basic.frag").c_str());
	std::vector<glm::vec3> verts, color;
	//front
	verts.push_back(glm::vec3(minX, maxY, minZ));
	verts.push_back(glm::vec3(minX, minY, minZ));
	verts.push_back(glm::vec3(maxX, minY, minZ));
	verts.push_back(glm::vec3(maxX, maxY, minZ));
	//top
	verts.push_back(glm::vec3(minX, maxY, minZ));
	verts.push_back(glm::vec3(minX, maxY, maxZ));
	verts.push_back(glm::vec3(maxX, maxY, maxZ));
	verts.push_back(glm::vec3(maxX, maxY, minZ));

	//right
	verts.push_back(glm::vec3(maxX, maxY, minZ));
	verts.push_back(glm::vec3(maxX, minY, maxZ));
	verts.push_back(glm::vec3(maxX, minY, maxZ));
	verts.push_back(glm::vec3(maxX, maxY, maxZ));

	//bottom
	verts.push_back(glm::vec3(minX, minY, minZ));
	verts.push_back(glm::vec3(minX, minY, maxZ));
	verts.push_back(glm::vec3(maxX, minY, maxZ));
	verts.push_back(glm::vec3(maxX, minY, minZ));

	//left
	verts.push_back(glm::vec3(minX, minY, minZ));
	verts.push_back(glm::vec3(minX, maxY, maxZ));
	verts.push_back(glm::vec3(minX, maxY, maxZ));
	verts.push_back(glm::vec3(minX, minY, maxZ));

	

	GLuint vao, ibo;
	GLuint buffers[1];
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, buffers);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);
	glBindAttribLocation(shader.GetProgram(), 0, "position");
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	shader.Use();

	glUniformMatrix4fv(modelHandle, 1, GL_FALSE, &transform->GetMatrix()[0][0]); // Send it to the GLSL file
	glUniformMatrix4fv(viewHandle, 1, GL_FALSE, &cam->GetViewMatrix()[0][0]); // Send it to the GLSL file
	glUniformMatrix4fv(projHandle, 1, GL_FALSE, &cam->GetProjMatrix()[0][0]); // Send it to the GLSL file

	glDrawArrays(GL_LINE_STRIP, 0, verts.size());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

bool BoxCollider::IsColliding(Collider collider)
{
	return false;
}

