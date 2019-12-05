#version 420
uniform sampler2D thisTexture;
uniform vec3 objectColor;
uniform vec3 lightPos;

out vec4 FragColor;

vec3 lightColor = vec3(1.f);

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;
in vec3 viewPos;

float ambientStrength = 0.1;

void main()
{
	vec3 ambient = ambientStrength * lightColor;
		
	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = lightColor * diff;

	float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
	vec3 specular = lightColor * spec;

	vec3 colorResult = (ambient + diffuse + specular) * objectColor;
	FragColor = texture(thisTexture, texCoord) * vec4(colorResult, 1.0);
} 