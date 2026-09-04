#include "VBO.h"

VBO::VBO()
{
    glGenBuffers(1, &ID);
    this->Bind();
}

VBO::VBO(GLfloat* vertices, GLsizeiptr size, GLenum usage)
{
    glGenBuffers(1, &ID);
    this->Bind();
    glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);
}

void VBO::Allocate(GLsizeiptr tot_size, GLenum usage)
{
    glBufferData(GL_ARRAY_BUFFER, tot_size, nullptr, GL_STATIC_DRAW);
}

void VBO::SubData(GLfloat* vertices, GLintptr offset, GLsizeiptr size, GLenum usage)
{
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices);
}

void VBO::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete()
{
    glDeleteBuffers(1, &ID);
}