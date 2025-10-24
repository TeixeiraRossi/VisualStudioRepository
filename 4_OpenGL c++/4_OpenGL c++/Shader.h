#pragma once
#ifndef SHADER_H
#define SHADER_H

#include<glm.hpp>
#include<string>
#include<unordered_map>

class Shader {
public:
	Shader(const std::string vertFilePath, std::string fragFilePath);
	~Shader();

	void CreateShaders();
	void Bind() const;
	void Unbind() const;

	void SendUniformData(const std::string& variableName, const glm::mat4& data);
	

private:
	unsigned int _programId;
	unsigned int _vertexShaderId, _fragmentShaderId;
	std::string _vertFilePath;
	std::string _fragFilePath;
	std::unordered_map<std::string, int> _uniformLocation;

	void CompileShaders();
	void LinkProgram();

	int GetUniformLocation(const std::string& variableName);
	std::string ReadProgramSource(const std::string filePath);
};

#endif // !1
