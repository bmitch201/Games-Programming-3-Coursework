#version 420
uniform sampler2D thisTexture;
uniform sampler2D thisNormal;

uniform vec3 lightDir;
uniform vec3 viewPos;
uniform vec4 lightColor;

out vec4 FragColor;

in vec2 texCoord;
in vec3 fragPos;
in mat3 TBN;

void main()
{
	vec3 tangentfragPos = TBN * fragPos;
	vec3 tangentviewPos = TBN * viewPos;
	vec3 tangentlightPos = TBN * lightDir;

	vec3 color = texture(thisTexture, texCoord).rgb;

	vec3 norm = texture(thisNormal, texCoord).rgb;
	norm = normalize(norm* 2.f -1.f);
	
	float ambientStrength = 0.1f;
	vec4 ambient = ambientStrength * vec4(color, 1.f);

	vec3 tangentlightDir = normalize(tangentlightPos - tangentfragPos);
	float diff = max(dot(norm, tangentlightDir), 0.0);
	vec4 diffuse = vec4(color, 1.f) * diff;

	vec3 viewDir = normalize(tangentviewPos - tangentfragPos);
	vec3 halfwayDir = normalize(tangentlightDir + viewDir);
	
	float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
	vec4 specular = lightColor * spec;

	FragColor = (ambient + diffuse + specular);
} 