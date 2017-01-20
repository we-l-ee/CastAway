#version 430 core
out vec4 FragColor;
in vec2 TexCoords;

layout(location = 100)uniform sampler2D image;

uniform float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
	vec4 result = texture(image, TexCoords); 

	FragColor = vec4(result);
}