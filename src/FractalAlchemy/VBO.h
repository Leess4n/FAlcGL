#pragma once

#include <glad.h>

class VBO
{
public:
    unsigned int ID;

    VBO(GLfloat* vertices, GLsizeiptr size, GLenum usage);
    void Bind();
    void Unbind();
    void Delete();
};