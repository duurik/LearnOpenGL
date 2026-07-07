#version 330 core
out vec4 FragColor;

in vec4 vertexPos;
in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;
vec2 FlipVertAxis = vec2(-1.0f, 1.0f);

void main()
{
   FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord * FlipVertAxis), 0.2);
}