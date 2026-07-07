#version 330 core
out vec4 FragColor;
in vec4 vertexPos;
in vec4 vertexColor;
void main()
{
   FragColor = vertexPos;
}