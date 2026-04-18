#include "Shader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>


std::string readFile(const char* path)
{
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// compilar programa
unsigned int compileProgram(const char* vSrc, const char* fSrc)
{
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vSrc, NULL);
    glCompileShader(vs);

    int success;
    char infoLog[512];

    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
        std::cout << "Vertex Error:\n" << infoLog << std::endl;
    }

    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fSrc, NULL);
    glCompileShader(fs);

    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fs, 512, NULL, infoLog);
        std::cout << "Fragment Error:\n" << infoLog << std::endl;
    }

    unsigned int program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "Link Error:\n" << infoLog << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vCode = readFile(vertexPath);
    std::string fCode = readFile(fragmentPath);

    ID = compileProgram(vCode.c_str(), fCode.c_str());
}


void Shader::use()
{
    glUseProgram(ID);
}


void Shader::reloadFromSource(const std::string& vertexPath, const std::string& fragmentCode)
{
    std::string vCode = readFile(vertexPath.c_str());

    unsigned int newProgram = compileProgram(vCode.c_str(), fragmentCode.c_str());

    if (newProgram != 0)
    {
        glDeleteProgram(ID);
        ID = newProgram;

        std::cout << "Shader recarregado com sucesso!\n";
    }
}

// UNIFORMS
void Shader::setFloat(const std::string& name, float value)
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, float x, float y)
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setInt(const std::string& name, int value)
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}