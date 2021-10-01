#version 330

in vec4 color;	                                                         
out vec4 FragColor;

uniform vec4 gColor;

void main()                                                             
{            
	vec4 dummy = gColor; // TODO : voir comment se passer d'un uniform sur l'un des shader
	FragColor = color;							
}