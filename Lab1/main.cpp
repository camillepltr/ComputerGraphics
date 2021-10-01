
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
GLint gScaleLocation, gColorLocation;
float myScale = 0.5;
vec4 color = vec4(1.0, 1.0, 0.0, 1.0);

// Shader Functions- click on + to expand
#pragma region SHADER_FUNCTIONS
static void AddShader(GLuint ShaderProgram, const char* path, GLenum ShaderType)
{
	// Load shader file as text
	string shaderContent;
	ifstream inputFileStream(PATH_TO_SHADERS + path, ios::in);
	if (inputFileStream.is_open()) {
		shaderContent = string(istreambuf_iterator<char>(inputFileStream), istreambuf_iterator<char>());
		inputFileStream.close();
	}
	else {
		cout << "could not open " << path << endl;
		return;
	}
	//Convert shader content text (string) to char*
	const char* shaderContentCStr = shaderContent.c_str();

	// create a shader object
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(0);
    }
	// Bind the source code to the shader, this happens before compilation
	glShaderSource(ShaderObj, 1, (const GLchar**)&shaderContentCStr, NULL);
	// compile the shader and check for errors
    glCompileShader(ShaderObj);
    GLint success;
	// check for shader related errors using glGetShaderiv
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }
	// Attach the compiled shader object to the program object
    glAttachShader(ShaderProgram, ShaderObj);
}

GLuint CompileShaders(const char* vertexShaderPath, const char *fragmentShaderPath)
{
	//Start the process of setting up our shaders by creating a program ID
	//Note: we will link all the shaders together into this ID
    GLuint shaderProgramID = glCreateProgram();
    if (shaderProgramID == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

	// Create two shader objects, one for the vertex, and one for the fragment shader
    AddShader(shaderProgramID, vertexShaderPath, GL_VERTEX_SHADER);
    AddShader(shaderProgramID, fragmentShaderPath, GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

	// After compiling all shader objects and attaching them to the program, we can finally link it
    glLinkProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
	}

	// program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
    glValidateProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
    glGetProgramiv(shaderProgramID, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }
    glUseProgram(shaderProgramID);

	// Uniforms
	gScaleLocation = glGetUniformLocation(shaderProgramID, "gScale");
	if (gScaleLocation == -1) {
		fprintf(stderr, "Error getting location of uniform 'gScale'\n");
		exit(1);
	}
	gColorLocation = glGetUniformLocation(shaderProgramID, "gColor");
	if (gColorLocation == -1) {
		fprintf(stderr, "Error getting location of uniform 'gColor'\n");
		exit(1);
	}

	return shaderProgramID;
}
#pragma endregion SHADER_FUNCTIONS

// VBO Functions - click on + to expand
#pragma region VBO_FUNCTIONS
void generateObjectBuffer(vec3 vertices[], vec4 colors[]) {
	GLuint numVertices = 3; 
	// Genderate 1 generic buffer object, called VBO
	GLuint VBO;
 	glGenBuffers(1, &VBO);
	// In OpenGL, we bind (make active) the handle to a target name and then execute commands on that target
	// Buffer will contain an array of vertices 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// After binding, we now fill our object with data, everything in "Vertices" goes to the GPU
	glBufferData(GL_ARRAY_BUFFER, numVertices*7*sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	// if you have more data besides vertices (e.g., vertex colours or normals), use glBufferSubData to tell the buffer when the vertices array ends and when the colors start
	glBufferSubData (GL_ARRAY_BUFFER, 0, numVertices*3*sizeof(GLfloat), vertices);
	glBufferSubData (GL_ARRAY_BUFFER, numVertices*3*sizeof(GLfloat), numVertices*4*sizeof(GLfloat), colors);

}

void linkCurrentBuffertoShader(GLuint shaderProgramID){
	GLuint numVertices = 3; 
	// find the location of the variables that we will be using in the shader program
	GLuint positionID = glGetAttribLocation(shaderProgramID, "vPosition");
	GLuint colorID = glGetAttribLocation(shaderProgramID, "vColor");
	// Have to enable this
	glEnableVertexAttribArray(positionID);
	// Tell it where to find the position data in the currently active buffer (at index positionID)
    glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Similarly, for the color data.
	glEnableVertexAttribArray(colorID);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(numVertices*3*sizeof(GLfloat)));
}
#pragma endregion VBO_FUNCTIONS

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




