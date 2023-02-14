#version 330 core
in vec4 TexCoord;

out vec4 color;

uniform vec4 _color;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
    //color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.5);
	color = _color;
}