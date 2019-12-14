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
	vec4 color = texture(thisTexture, texCoord).rgba;

	vec4 ambient = ambientStrength * color;

	float diff = max(dot(normalize(normal), normalize(lightDir)), 0.0);
	vec4 diffuse = color * diff;

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
	vec4 specular = lightColor * spec;

	FragColor = (ambient + diffuse + specular);
} 