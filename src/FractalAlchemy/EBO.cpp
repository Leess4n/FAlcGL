#include "EBO.h"

EBO::EBO(GLuint* indices, GLsizeiptr size, GLenum usage)
{
    glGenBuffers(1, &ID);
    this->Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, usage);
}

void EBO::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete() const
{
    glDeleteBuffers(1, &ID);
}