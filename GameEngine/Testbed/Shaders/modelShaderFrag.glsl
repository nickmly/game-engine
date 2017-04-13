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


vec3 calcPointLight(PointLight light)
{
	// Attenuation
	float dist = length(light.position - FragPos);
	float attenuation = 1.0f / (light.constant + light.linear * dist + 
		light.quadratic * (dist * dist)); // Formula is used to make point light fade away at a certain distance

	// Ambient 
	vec3 ambient = attenuation * light.ambient * vec3(texture(texture_diffuse1, TexCoords));
	
	// Diffuse
	vec3 norm = normalize(Normal);	
	vec3 lightDir = normalize(light.position - FragPos); // Direction vector between light source and fragment's position
	float diff = max(dot(norm,lightDir), 0.0); // Use dot product of both vectors to get the impact of the diffuse light (use max to avoid this value ever being negative)
	vec3 diffuse = attenuation * light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
	
	// Specular
	vec3 viewDir = normalize(cameraPosition - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = attenuation * light.specular * spec * material.specular;
	
	vec3 result = ambient + diffuse + specular;
	return result;
}

vec3 calcDirLight(DirLight light)
{
	// Ambient 
	vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
	
	// Diffuse
	vec3 norm = normalize(Normal);	
	vec3 lightDir = normalize(-light.direction); 
	float diff = max(dot(norm,lightDir), 0.0); // Use dot product of both vectors to get the impact of the diffuse light (use max to avoid this value ever being negative)
	vec3 diffuse =  light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
		
	// Specular
	vec3 viewDir = normalize(cameraPosition - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);	
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * material.specular;
	
	vec3 result = ambient + diffuse + specular;
	return result;
}

void main()
{	
	vec3 result = calcDirLight(directionalLight);
	for(int i = 0; i < POINT_LIGHT_COUNT; i++)
	{
		result += calcPointLight(pointLights[i]);
	}
	
	fColor = vec4(texture(texture_diffuse1, TexCoords)) * vec4(result,1.0f);
}

