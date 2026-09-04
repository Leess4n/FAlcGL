#pragma once

#include <glad.h>

class VBO
{
public:
    VBO();
    VBO(GLfloat* vertices, GLsizeiptr size, GLenum usage);

    void Allocate(GLsizeiptr tot_size, GLenum usage);
    void SubData(GLfloat* vertices, GLintptr offset, GLsizeiptr size, GLenum usage);
    void Bind();
    void Unbind();
    void Delete();

public:
    unsigned int ID;
};