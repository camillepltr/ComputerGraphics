/*******************************************************************************************
	Computer Graphics - TCD
	Camille Peltier (peltierc@tcd.ie)
	2021
	-------------------
	Shader.cpp
********************************************************************************************/
#include "Shader.h"

// Constructors
Shader::Shader() {
	shaderProgramID = 0;
}

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
	//Create program ID. We will link all the shaders together into this ID
	shaderProgramID = glCreateProgram();
	if (shaderProgramID == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}
	// Load vertex and fragment shaders' code from text files
	addShader(vertexShaderPath, GL_VERTEX_SHADER);
	addShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	// After compiling all shader objects and attaching them to the program, we can finally link it
	glLinkProgram(shaderProgramID);
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	// Needs to be validated to check whether the program can execute given the current pipeline state
	glValidateProgram(shaderProgramID);
	glGetProgramiv(shaderProgramID, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}
	glUseProgram(shaderProgramID);
}

// Public methods
void Shader::use() {
	glUseProgram(shaderProgramID);
}

GLint Shader::getUniformLocation(const string& uniformName) const {
	GLint gLocation = glGetUniformLocation(shaderProgramID, uniformName.c_str());
	if (gLocation == -1) {
		fprintf(stderr, "Error getting location of uniform '%s'\n", uniformName.c_str());
		exit(1);
	}
	return gLocation;
}

void Shader::setUniformBool(const string& uniformName, bool value) const {
	GLint gLocation = getUniformLocation(uniformName);
	glUniform1i(gLocation, (int)value);
}


void Shader::setUniform1i(const string& uniformName, int value) const {
	GLint gLocation = getUniformLocation(uniformName);
	glUniform1i(gLocation, value);
}

void Shader::setUniform1f(const string& uniformName, float value) const {
	GLint gLocation = getUniformLocation(uniformName);
	glUniform1f(gLocation, value);
}

void Shader::setUniform4fv(const string& uniformName, int n, vec4 value) const {
	GLint gLocation = getUniformLocation(uniformName);
	glUniform4fv(gLocation, n, value_ptr(value));
}

// Protected methods
string Shader::loadShaderFromFile(const char* path) {
	string shaderContent;
	ifstream inputFileStream(PATH_TO_SHADERS + path, ios::in);
	if (inputFileStream.is_open()) {
		shaderContent = string(istreambuf_iterator<char>(inputFileStream), istreambuf_iterator<char>());
		inputFileStream.close();
	}
	else {
		cout << "could not open " << path << endl;
		exit(1);
	}
	return shaderContent;
}

void Shader::addShader(const char* path, GLenum ShaderType) {
	// Load shader code
	string shaderContent = loadShaderFromFile(path);
	const char* shaderCode = shaderContent.c_str();

	// Create a shader object
	GLuint shaderObj = glCreateShader(ShaderType);
	if (shaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}

	// Bind the source code to the shader
	glShaderSource(shaderObj, 1, (const GLchar**)&shaderCode, NULL);
	// Compile shader
	glCompileShader(shaderObj);
	GLint success;
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(shaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		exit(1);
	}

	// Attach the compiled shader object to the program object
	glAttachShader(shaderProgramID, shaderObj);
	glDeleteShader(shaderObj);
}
