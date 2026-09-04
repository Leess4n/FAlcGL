#pragma once

#include <glad.h>

class EBO
{
public:
    unsigned int ID;

    EBO(GLuint* indices, GLsizeiptr size, GLenum usage);
    void Bind() const;
    void Unbind() const;
    void Delete() const;
};