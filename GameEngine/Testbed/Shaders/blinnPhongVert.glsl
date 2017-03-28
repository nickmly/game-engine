#version 330
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 vertNormal;
out vec3 lightDir;
 
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightPos;

void main() {

	vertNormal = normalMatrix * vec4(normal,1.0f).xyz; /// Rotate the normal to the correct orientation 
	vec4 eyePos = modelViewMatrix * vec4(position,1.0f); /// Create the eye vector 
	lightDir = normalize(lightPos - eyePos.xyz); /// Create the light direction 

	gl_Position =  projectionMatrix * modelViewMatrix * vec4(position,1.0f); 
}
