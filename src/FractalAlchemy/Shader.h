#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad.h>
#include <glm/glm.hpp>

#include "Utility.h"

class Shader
{
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath);
    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec4(const std::string &name, float v0, float v1, float v2, float v3) const;
    void setMat4(const std::string &name, glm::mat4 mat);
};
