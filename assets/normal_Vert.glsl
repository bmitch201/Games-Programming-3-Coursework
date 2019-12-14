#version 420
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aCoord;
layout(location = 3) in vec3 aNormal;
layout(location = 4) in vec3 aTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;
out vec4 vertexColor;
out vec3 fragPos;
out mat3 TBN;

void main()
{
	vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	TBN = transpose(mat3(T, B, N));

	texCoord = aCoord;
	fragPos = (model * vec4(aPos, 1.0)).xyz;
	gl_Position = projection * view * vec4(fragPos, 1.0);
}