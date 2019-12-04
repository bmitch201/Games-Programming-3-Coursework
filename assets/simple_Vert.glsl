#version 420
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aCoord;

uniform vec3 objectColor;
uniform mat4 MVP;  

out vec2 texCoord;
out vec4 vertexColor;

void main()
{
    gl_Position = MVP * vec4(aPos, 1.0);
	texCoord = aCoord;
    vertexColor = vec4(objectColor, 1.0);
}