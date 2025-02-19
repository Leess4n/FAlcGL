#include "VBO.h"

VBO::VBO(GLfloat* vertices, GLsizeiptr size, GLenum usage)
{
    glGenBuffers(1, &ID);
    this->Bind();
    glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);
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