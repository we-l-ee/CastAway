#version 430 core

in vec2 TexCoord;
uniform sampler2D scene;


out vec4 color;
void main(){

	color = vec4(texture(scene, TexCoord), 1.0);
	
}