#version 430 core

layout(location = 100) uniform sampler2D diffuse;

smooth in vec2 texCoord;

layout(location = 0) out vec4 fcolor;
layout(location = 1) out vec4 blurcolor;


void main(void) {
	fcolor = vec4(vec3(0),1.0);
	blurcolor =  vec4(1.0);
}