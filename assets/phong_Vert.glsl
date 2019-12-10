#version 420
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aCoord;
layout(location = 3) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 MVP;  

out vec2 texCoord;
out vec3 normal;
out vec4 vertexColor;
out vec3 fragPos;

void main()
{
	texCoord = aCoord;
	normal = aNormal;
	fragPos = aPos;

	gl_Position = projection * view * vec4(fragPos, 1.0);
}