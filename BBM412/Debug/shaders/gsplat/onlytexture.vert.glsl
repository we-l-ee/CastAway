#version 430 core

layout(location = 0)in vec3 vertex;
layout(location = 1)in vec2 tex_coord;
layout(location = 2)in vec3 normal;

layout(location = 10)uniform mat4 MVP;
layout(location = 11)uniform mat4 MV;


out vec2 texCoord;
out vec2 splatTexCoord;


void main(void) {
	texCoord = vec2(vertex.x, vertex.z);	
	gl_Position = MVP*vec4(vertex, 1.0f);
	splatTexCoord = vec2(tex_coord.x, 1 - tex_coord.y);

}