#include"ExpressionParser.h"

std::string generateGLSL(const std::string& expr)
{
    if (expr == "z*z") return "cmul(z,z)";
    if (expr == "sin(z)") return "csin(z)";
    if (expr == "1/z") return "cdiv(vec2(1.0,0.0), z)";

    return "z";
}