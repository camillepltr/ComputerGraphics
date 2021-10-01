#version 330

// yellow Fragment Shader (EX 2.4 of Lab 1)                       
out vec4 FragColor;
uniform vec4 gColor;

void main()
{
	FragColor = gColor;
}