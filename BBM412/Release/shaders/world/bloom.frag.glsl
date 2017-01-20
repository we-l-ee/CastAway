#version 430 core
out vec4 fColor;
in vec2 TexCoords;

layout(location = 100)uniform sampler2D scene;
layout(location = 101)uniform sampler2D bloomBlur;


void main()
{
	vec3 result ;
	vec3 sceneColor = texture(scene, TexCoords).rgb;
	vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;

	result = sceneColor + bloomColor; 
	fColor = vec4(	result,	1.0);

}