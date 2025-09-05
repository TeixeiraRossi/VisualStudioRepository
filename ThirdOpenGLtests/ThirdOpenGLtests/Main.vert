#version 460

uniform mat4 matrix;	

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 colorIn;

out vec3 colorOut;

void main() {
	gl_Position = matrix*vec4(position, 1.0);

	colorOut = colorIn;
}