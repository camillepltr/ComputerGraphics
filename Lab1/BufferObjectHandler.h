/*******************************************************************************************
    Computer Graphics - TCD
    Camille Peltier (peltierc@tcd.ie)
    2021
    -------------------
    BufferObjectHandler.h
    This class represent a Vertex Object Buffer, to set it up easily
********************************************************************************************/
#pragma once
#ifndef VBO_H
#define VBO_H

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Shader.h"

using namespace std;
using namespace glm;

class BufferObjectHandler
{
public:
    // Constructors
    BufferObjectHandler();

    // Public methods
    static void generateBufferObject(GLint numVertices, vec3 vertices[], vec4 colors[]);
    static void linkCurrentBufferToShader(Shader shader, GLint numVertices);

protected:
};

#endif



