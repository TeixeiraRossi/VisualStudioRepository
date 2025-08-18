#version 460

//uniform vec3 triangleColor;
uniform float u_deltaTime;

in vec3 colorOut;

out vec4 fragColor;

void main() {
	float valuex=abs(sin(u_deltaTime/8.f));
	float valuey=abs(sin(u_deltaTime/4.f));
	float valuez=abs(sin(u_deltaTime/2.f));

	fragColor  = vec4(colorOut, 1.f);
}