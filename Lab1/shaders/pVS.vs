#version 330                                                                 

in vec3 vPosition;															  
in vec4 vColor;										
out vec4 color;	

uniform float gScale;

void main()                                                                    
{
	gl_Position = vec4(gScale*vPosition.x, gScale*vPosition.y, vPosition.z, 1.0);
	color = vColor;						
}