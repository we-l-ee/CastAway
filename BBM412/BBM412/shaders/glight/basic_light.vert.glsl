#version 430 core

layout(location = 0)in vec3 vertex;

layout(location = 1)uniform vec4 color;

layout(location = 10)uniform mat4 mvp;



out vec4 vcolor;

void main(void) {

	gl_Position = mvp*vec4(vertex, 1.0f);
	vcolor = color;
	
}