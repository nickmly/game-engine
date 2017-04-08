#version 330 core
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 fColor;
uniform sampler2D texture_diffuse1;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

void main()
{
	// Ambient 
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;
	
	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPosition - FragPos); // Direction vector between light source and fragment's position
	
	float diff = max(dot(norm,lightDir), 0.0); // Use dot product of both vectors to get the impact of the diffuse light (use max to avoid this value ever being negative)
	vec3 diffuse = diff * lightColor;

	// Specular
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(cameraPosition - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * vec3(1.0f);
	
	vec3 result = ambient + diffuse + specular;
	fColor = vec4(texture(texture_diffuse1, TexCoords)) * vec4(result,1.0f);
}