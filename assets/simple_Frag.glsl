#version 420
uniform sampler2D thisTexture;

out vec4 FragColor;
  
in vec2 texCoord;
in vec4 vertexColor;

void main()
{
	FragColor = texture(thisTexture, texCoord) * vertexColor;
} 