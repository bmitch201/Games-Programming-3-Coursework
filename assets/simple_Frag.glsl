#version 420
uniform sampler2D thisTexture;
uniform vec3 objectColor;

out vec4 FragColor;

in vec2 texCoord;

void main()
{
	FragColor = texture(thisTexture, texCoord) * vec4(objectColor, 1.0);
} 