#version 430 core

layout(location = 1)uniform vec4 color;

layout(location = 0) out vec4 fcolor;
layout(location = 1) out vec4 blurcolor;

void main(void) {

	 blurcolor = color;
	 fcolor =color;//vec4(0);
}