#version 430 core

layout(location = 0)in vec3 vertex;
layout(location = 0)in vec2 text_coord;


layout(location = 10)uniform mat4 mvp;



out vec2 texCoord;

void main(void) {

	gl_Position = mvp*vec4(vertex, 1.0f);
	texCoord = text_coord;
}