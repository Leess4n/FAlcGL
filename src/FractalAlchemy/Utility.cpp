#include "Utility.h"

void util::printError(unsigned int ID, GLenum pname, const char* errMsg)
{
    int success;
    const unsigned int len = 512;
    char infoLog[len];

    glGetShaderiv(ID, pname, &success);
    if(!success)
    {
        glGetShaderInfoLog(ID, len, NULL, infoLog);
        std::cout << errMsg << "\n" << infoLog << std::endl;
    }
}