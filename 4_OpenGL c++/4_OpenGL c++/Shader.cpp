#include<GL/glew.h>
#include<fstream>
#include<sstream>
#include<gtc/type_ptr.hpp>

#include"Shader.h"
#include"Error.h"

Shader::Shader(const std::string vertFilePath, const std::string fragFilePath) :
	_programId(0),
	_vertexShaderId(0),
	_fragmentShaderId(0),

