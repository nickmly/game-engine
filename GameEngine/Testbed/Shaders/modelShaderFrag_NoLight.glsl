#version 330 core
struct Material 
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess; 
};

struct PointLight 
{
	vec3 position;

	vec3 diffuse;
	vec3 ambient;
	vec3 specular;

	float constant, linear, quadratic;
};

struct DirLight 
{
	vec3 direction;
	
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
};


in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 fColor;
uniform Material material;
uniform sampler2D texture_diffuse1;
uniform vec3 cameraPosition;

#define POINT_LIGHT_COUNT 2

uniform PointLight pointLights[POINT_LIGHT_COUNT]; // Array must be filled or model will appear black
uniform DirLight directionalLight;


void main()
{		
	fColor = vec4(texture(texture_diffuse1, TexCoords));
}

