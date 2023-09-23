#pragma once
#include <string>
#include <glad/glad.h>


class GLUtils
{
public:
    static GLuint LoadShaderFromFile(const std::string &path, GLenum shaderType);
};
