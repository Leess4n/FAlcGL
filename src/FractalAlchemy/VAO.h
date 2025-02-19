#pragma once

#include <glad.h>
#include "VBO.h"

class VAO
{
public:
    unsigned int ID;

    VAO();
    void LinkAttrib(VBO &VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) const;
    void Bind() const;
    void Unbind() const;
    void Delete() const;
};