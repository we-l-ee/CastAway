#version 430 core

layout(location = 100) uniform sampler2D diffuse;
in vec2 texCoord;

out vec4 fcolor;

void main(void) {
	fcolor = texture(diffuse, texCoord);
}