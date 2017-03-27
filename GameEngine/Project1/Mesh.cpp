#include "Mesh.h"
#include "Vertex.h"
#include "FPS_Camera.h"


#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/matrix_inverse.hpp>
#include <string>
#include <sstream>


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


void Mesh::Render(glm::mat4 transform, glm::vec3 lightPos)
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
	
	glUniformMatrix4fv(modelHandle, 1, GL_FALSE, &transform[0][0]); // Send it to the GLSL file
	glUniformMatrix4fv(viewHandle, 1, GL_FALSE, &camera->GetViewMatrix()[0][0]); // Send it to the GLSL file
	glUniformMatrix4fv(projHandle, 1, GL_FALSE, &camera->GetProjMatrix()[0][0]); // Send it to the GLSL file


	glUniform3f(glGetUniformLocation(shader.GetProgram(), "diffuse_color"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shader.GetProgram(), "specular_color"), 1.0f, 0.5f, 0.0f);
	glUniform3f(glGetUniformLocation(shader.GetProgram(), "light_position"), lightPos.x, lightPos.y, lightPos.z);

	glUniform3f(glGetUniformLocation(shader.GetProgram(), "vEyeSpaceCameraPosition"), camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);
	
	glm::mat3 MV = transform * camera->GetViewMatrix();
	glUniformMatrix3fv(glGetUniformLocation(shader.GetProgram(), "lightNormal"), 1, GL_FALSE, glm::value_ptr(glm::inverseTranspose(MV)));
	
	// Draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Always good practice to set everything back to defaults once configured.
	for (GLuint i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}

