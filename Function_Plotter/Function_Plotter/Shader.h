#pragma once

#include <string>

class Shader
{
public:
    unsigned int ID;

    // construtor (carrega arquivos)
    Shader(const char* vertexPath, const char* fragmentPath);

    void use();

    // uniforms
    void setFloat(const std::string& name, float value);
    void setVec2(const std::string& name, float x, float y);
};