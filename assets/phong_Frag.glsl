#version 420
uniform sampler2D thisTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;



void main()
{
	vec3 color = texture(thisTexture, texCoord).rgb;
	
	float ambientStrength = 0.1f;
	vec4 ambient = ambientStrength * color;

	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 norm = normalize(normal);
	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = color * diff;

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	
	float specStrength = 0.5f;
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	vec3 specular = lightColor * spec;

	FragColor = vec4(ambient + diffuse + specular, 1.0);
} 