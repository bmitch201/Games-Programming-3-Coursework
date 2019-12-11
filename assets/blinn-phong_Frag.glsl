#version 420
uniform sampler2D thisTexture;
uniform vec3 lightDir;
uniform vec3 viewPos;
uniform vec4 lightColor;

out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

float ambientStrength = 0.2f;

void main()
{
	vec4 ambient = ambientStrength * lightColor;

	float diff = max(dot(normalize(normal), normalize(lightDir)), 0.0);
	vec4 diffuse = lightColor * diff;

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
	vec4 specular = lightColor * spec;

	FragColor = texture(thisTexture, texCoord) * (ambient + diffuse + specular);
} 