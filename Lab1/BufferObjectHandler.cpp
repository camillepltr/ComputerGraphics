/*******************************************************************************************
    Computer Graphics - TCD
    Camille Peltier (peltierc@tcd.ie)
    2021
    -------------------
    BufferObjectHandler.cpp
    This class represent a shader program, which has a vertex shader and a fragment shader
********************************************************************************************/
#include "BufferObjectHandler.h"

// Constructors
BufferObjectHandler::BufferObjectHandler() {}

// Public methods
void BufferObjectHandler::generateBufferObject(GLint numVertices, vec3 vertices[], vec4 colors[]) {
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, numVertices * 7 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * 3 * sizeof(GLfloat), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(GLfloat), numVertices * 4 * sizeof(GLfloat), colors);
}

void BufferObjectHandler::linkCurrentBufferToShader(Shader shader, GLint numVertices) {
    GLuint positionID = glGetAttribLocation(shader.shaderProgramID, "vPosition");
    GLuint colorID = glGetAttribLocation(shader.shaderProgramID, "vColor");
    // Enable
    glEnableVertexAttribArray(positionID);
    glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // Similarly, for the color data.
    glEnableVertexAttribArray(colorID);
    glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(numVertices * 3 * sizeof(GLfloat)));
}
