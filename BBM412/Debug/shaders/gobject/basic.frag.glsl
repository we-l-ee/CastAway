#version 430 core

in vec4 vcolor;

layout(location=0)out vec4 fcolor;

void main(void) {

	fcolor = vcolor;

}