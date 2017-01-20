#version 430 core

layout (location = 0) out vec4 fColor;
layout(location = 1) out vec4 blurColor;


in vec2 texCoord;
in vec2 splatTexCoord;

layout(location = 100) uniform sampler2D text1;
layout(location = 101) uniform sampler2D text2;
layout(location = 102) uniform sampler2D text3;
layout(location = 103) uniform sampler2D text4;

layout(location = 104) uniform sampler2D splat;

const float tile = 0.1;

void main(void) {
	vec4 alpha = texture(splat, splatTexCoord);

	vec3 color1 = vec3( texture(text1, texCoord*tile).rgb );
	vec3 color2 = vec3( texture(text2, texCoord*tile).rgb );
	vec3 color3 = vec3( texture(text3, texCoord*tile).rgb );
	vec3 color4 = vec3( texture(text4, texCoord*tile).rgb );

	vec3 color = color1*alpha.r + color2*alpha.g + color3*alpha.b + color4*(1 - alpha.a);


	fColor = //alpha;
		vec4( color, 1.0 );
		//vec4(alpha.a);
	blurColor = vec4(vec3(0), 1.0);
}