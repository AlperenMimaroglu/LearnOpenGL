#include "GLUtils.h"

#include <fstream>
#include <iostream>

GLuint GLUtils::LoadShaderFromFile(const std::string& path, GLenum shaderType)
{
    GLuint shaderID = 0;
    std::string shaderString;
    std::ifstream sourceFile(path.c_str());

    if (sourceFile)
    {
        shaderString.assign(std::istreambuf_iterator<char>(sourceFile), std::istreambuf_iterator<char>());

        shaderID = glCreateShader(shaderType);

        const GLchar* shaderSource = shaderString.c_str();
        glShaderSource(shaderID, 1, &shaderSource, nullptr);

        glCompileShader(shaderID);

        GLint shaderCompiled = GL_FALSE;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
        if (shaderCompiled != GL_TRUE)
        {
            std::cerr << "Unable to compile shader: " << shaderID << " Source: " << shaderSource << std::endl;

            char infoLog[512];
            glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;

            glDeleteShader(shaderID);
            shaderID = 0;
        }
    }
    else
    {
        std::cerr << "Unable to open file: " << path << std::endl;
    }

    return shaderID;
}
