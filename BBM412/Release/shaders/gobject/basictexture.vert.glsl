#version 430 core

layout(location = 0)in vec3 vertex;
layout(location = 1)in vec2 text_coord;


layout(location = 10)uniform mat4 mvp;



smooth out vec2 texCoord;

void main(void) {

	gl_Position = mvp*vec4(vertex, 1.0f);
	texCoord = vec2(text_coord.x, text_coord.y);
}