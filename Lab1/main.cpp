
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>

#include "Shader.h"
#include "BufferObjectHandler.h"

// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;
using namespace glm;

GLuint VAOs[2];
Shader firstShader, secondShader;

float myScale = 0.5;
vec4 color = vec4(1.0, 1.0, 0.0, 1.0);

void specialKeyboardEvent(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP:
			color.r += 0.01f; break;
		case GLUT_KEY_RIGHT:
			color.g += 0.01f; break;
		case GLUT_KEY_DOWN:
			color.r -= 0.01f; break;
		case GLUT_KEY_LEFT:
			color.g -= 0.01f; break;
	}
}

void mouseEvent(int button, int state, int x, int y) {
	switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_UP) {
				myScale += 0.01f;
			}
			break;
		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_UP) { 
				myScale -= 0.01f;
			}
			break;
	}
}

void display() {
	// Clearing the window
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Triangle 1
	firstShader.use();
	glBindVertexArray(VAOs[0]);
	firstShader.setUniform1f("gScale", myScale);
	firstShader.setUniform4fv("gColor", 1, color);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//Triangle 2
	secondShader.use();
	glBindVertexArray(VAOs[1]);
	secondShader.setUniform1f("gScale", myScale);
	secondShader.setUniform4fv("gColor", 1, color);
	glDrawArrays(GL_TRIANGLES, 0, 3);

    glutSwapBuffers();
}


void init()
{
	glGenVertexArrays(2, VAOs);

	vec3 triangle1[] = { vec3(-1.0f, -1.0f, 0.0f),
		vec3(-0.5f, 1.0f, 0.0f),
		vec3(0.0f, -1.0f, 0.0f )};
	vec4 colors[] = { vec4(0.0f, 1.0f, 0.0f, 1.0f),
		vec4(1.0f, 0.0f, 0.0f, 1.0f),
		vec4(0.0f, 0.0f, 1.0f, 1.0f) };
	vec3 triangle2[] = { vec3(0.0f, -1.0f, 0.0f),
		vec3(0.5f, 1.0f, 0.0f),
		vec3(1.0f, -1.0f, 0.0f) };

	//Triangle 1
	glBindVertexArray(VAOs[0]);
	firstShader = Shader("pVS.vs", "pFS.fs");
	BufferObjectHandler::generateBufferObject(3, triangle1, colors);
	BufferObjectHandler::linkCurrentBufferToShader(firstShader, 3);

	//Triangle 2
	glBindVertexArray(VAOs[1]);
	secondShader = Shader("pVS.vs", "pFS2.fs");
	BufferObjectHandler::generateBufferObject(3, triangle2, colors);
	BufferObjectHandler::linkCurrentBufferToShader(secondShader, 3);
}

int main(int argc, char** argv){

	// Set up the window
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Lab 2");

	// Set the callbacks
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutSpecialFunc(specialKeyboardEvent);
	glutMouseFunc(mouseEvent);

	 // A call to glewInit() must be done after glut is initialized!
    GLenum res = glewInit();
	// Check for any errors
    if (res != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return 1;
    }
	// Set up your objects and shaders
	init();
	// Begin infinite event loop
	glutMainLoop();
    return 0;
}




