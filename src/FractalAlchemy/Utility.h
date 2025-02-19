#pragma once

#include <glad.h>
#include <iostream>

namespace util
{
    void printError(unsigned int ID, GLenum pname, const char* errMsg);
}