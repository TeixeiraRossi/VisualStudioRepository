#include "Shader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

static std::string readFile(const char* path)
{
    std::cout << "Tentando abrir: " << path << std::endl;
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cout << "Erro ao abrir shader: " << path << std::endl;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vCode = readFile(vertexPath);
    std::string fCode = readFile(fragmentPath);

    const char* vSrc = vCode.c_str();
    const char* fSrc = fCode.c_str();

    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vSrc, NULL);
    glCompileShader(vs);

    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fSrc, NULL);
    glCompileShader(fs);


    int success;
    char infoLog[512];

    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
        std::cout << "Erro Vertex Shader:\n" << infoLog << std::endl;
    }

    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fs, 512, NULL, infoLog);
        std::cout << "Erro Fragment Shader:\n" << infoLog << std::endl;
    }


    ID = glCreateProgram();
    glAttachShader(ID, vs);
    glAttachShader(ID, fs);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "Erro Link Program:\n" << infoLog << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
}

void Shader::use()
{
    glUseProgram(ID);
}


void Shader::setFloat(const std::string& name, float value)
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, float x, float y)
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}