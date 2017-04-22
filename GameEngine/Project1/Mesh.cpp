#include "Mesh.h"
#include "Light.h"
#include "Vertex.h"
#include "FPS_Camera.h"


#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/matrix_inverse.hpp>
#include <string>
#include <sstream>

#pragma warning(disable:4996)

Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<GLuint> _indices, std::vector<Texture> _textures, Shader _shader, FPS_Camera* _camera)
{
	vertices = _vertices;
	indices = _indices;
	textures = _textures;

	shader = _shader;
	camera = _camera;

	modelHandle = glGetUniformLocation(shader.GetProgram(), "model"); // Assign modelHandle to uniform value model in shader program
	viewHandle = glGetUniformLocation(shader.GetProgram(), "view");
	projHandle = glGetUniformLocation(shader.GetProgram(), "projection");

	SetupMesh();
}

Mesh::~Mesh()
{
}


void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
		&vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
		&indices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, pos));

	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, normal));

	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, texCoord));

	printf("SetupMesh\n");
	glBindVertexArray(0);
}


// TODO: Find better way to pass lights through to this
void Mesh::Render(Transform transform, Light dirLight, std::vector<Light> lights, GLuint skyboxTexture)
{

	// Bind appropriate textures
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	for (GLuint i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
										  // Retrieve texture number (the N in diffuse_textureN)
		std::stringstream ss;
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			ss << diffuseNr++; // Transfer GLuint to stream
		else if (name == "texture_specular")
			ss << specularNr++; // Transfer GLuint to stream
		number = ss.str();
		// Now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader.GetProgram(), (name + number).c_str()), i);
		// And finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	// Send model, view and projection data to GLSL file
	glUniformMatrix4fv(modelHandle, 1, GL_FALSE, &transform.GetMatrix()[0][0]);
	glUniformMatrix4fv(viewHandle, 1, GL_FALSE, &camera->GetViewMatrix()[0][0]);
	glUniformMatrix4fv(projHandle, 1, GL_FALSE, &camera->GetProjMatrix()[0][0]);

	// Send light data to GLSL file
	if (dirLight.direction.w == 1.0f) // Directional Light
	{
		glUniform3f(glGetUniformLocation(shader.GetProgram(), "directionalLight.position"), dirLight.position.x, dirLight.position.y, dirLight.position.z);
		glUniform3f(glGetUniformLocation(shader.GetProgram(), "directionalLight.direction"), dirLight.direction.x, dirLight.direction.y, dirLight.direction.z);
		glUniform3f(glGetUniformLocation(shader.GetProgram(), "directionalLight.ambient"), dirLight.ambient.x, dirLight.ambient.y, dirLight.ambient.z);
		glUniform3f(glGetUniformLocation(shader.GetProgram(), "directionalLight.diffuse"), dirLight.diffuse.x, dirLight.diffuse.y, dirLight.diffuse.z);
		glUniform3f(glGetUniformLocation(shader.GetProgram(), "directionalLight.specular"), dirLight.specular.x, dirLight.specular.y, dirLight.specular.z);

	}

	//TODO: Come up with faster way that doesn't involve using glGetUniformLocation every frame
	for (int i = 0; i < lights.size(); i++)
	{
		Light light = lights[i];

		std::string currentLight = std::string("pointLights[" + std::to_string(i) + "]");
		glUniform3f(glGetUniformLocation(shader.GetProgram(), std::string(currentLight + ".position").c_str()), light.position.x, light.position.y, light.position.z);
		glUniform3f(glGetUniformLocation(shader.GetProgram(), std::string(currentLight + ".ambient").c_str()), light.ambient.x, light.ambient.y, light.ambient.z);
		glUniform3f(glGetUniformLocation(shader.GetProgram(), std::string(currentLight + ".diffuse").c_str()), light.diffuse.x, light.diffuse.y, light.diffuse.z);
		glUniform3f(glGetUniformLocation(shader.GetProgram(), std::string(currentLight + ".specular").c_str()), light.specular.x, light.specular.y, light.specular.z);
		glUniform1f(glGetUniformLocation(shader.GetProgram(), std::string(currentLight + ".constant").c_str()), light.constant);
		glUniform1f(glGetUniformLocation(shader.GetProgram(), std::string(currentLight + ".linear").c_str()), light.linear);
		glUniform1f(glGetUniformLocation(shader.GetProgram(), std::string(currentLight + ".quadratic").c_str()), light.quadratic);

	}


	//TODO: this is not used when you have diffuse maps
	// Send material data to GLSL file
	glUniform3f(glGetUniformLocation(shader.GetProgram(), "material.ambient"), 0.1f, 0.1f, 0.1f);
	glUniform3f(glGetUniformLocation(shader.GetProgram(), "material.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shader.GetProgram(), "material.specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shader.GetProgram(), "material.shininess"), 32.0f);

	// Send camera position to GLSL file
	glUniform3f(glGetUniformLocation(shader.GetProgram(), "cameraPosition"), camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);
	glm::mat3 normalMat = transform.GetMatrix();
	glUniformMatrix3fv(glGetUniformLocation(shader.GetProgram(), "normalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(normalMat))));

	// Draw mesh
	glBindVertexArray(VAO);

	//Refraction
	glActiveTexture(GL_TEXTURE3); // We already have 3 texture units active (in this shader) so set the skybox as the 4th texture unit (texture units are 0 based so index number 3)
	glUniform1i(glGetUniformLocation(shader.GetProgram(), "skybox"), 3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	//

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Unbind any active textures
	for (GLuint i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}

