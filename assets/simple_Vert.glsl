#version 420
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aCoord;

uniform mat4 MVP;  

out vec2 texCoord;

void main()
{
	texCoord = aCoord;
	gl_Position = MVP * vec4(aPos, 1.0);
}