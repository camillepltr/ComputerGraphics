/*******************************************************************************************
    Computer Graphics - TCD
    Camille Peltier (peltierc@tcd.ie)
    2021
    -------------------
    Shader.h
    This class represent a shader program, which has a vertex shader and a fragment shader
********************************************************************************************/
#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
using namespace glm;

const string PATH_TO_SHADERS = "../Lab1/shaders/";

class Shader
{
public:
    // Attributes
    GLuint shaderProgramID;

    // Public methods
    void use();
    GLint getUniformLocation(const string& uniformName) const;
    void setUniformBool(const string& uniformName, bool value) const;
    void setUniform1i(const string& uniformName, int value) const;
    void setUniform1f(const string& uniformName, float value) const;
    void setUniform4fv(const string& uniformName, int n, vec4 value) const;

    // Constructors
    Shader(); // Default constructor
    Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

protected:
    // Protected methods
    string loadShaderFromFile(const char* path);
    void addShader(const char* path, GLenum ShaderType);
};

#endif

