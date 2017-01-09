#version 330 core
out vec4 fColor;
in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;

//layout(location=10)uniform float exposure;

void main()
{
	vec3 result;
	//const float gamma = 2.2;
	vec3 hdrColor = texture(scene, TexCoords).rgb;
	vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;

	result = hdrColor + bloomColor; // additive blending
								// tone mapping
	// result = vec3(1.0) - exp(-hdrColor * exposure);
	// also gamma correct while we're at it       
	//result = pow(result, vec3(1.0 / gamma));
	fColor = vec4(result, 1.0f);

}