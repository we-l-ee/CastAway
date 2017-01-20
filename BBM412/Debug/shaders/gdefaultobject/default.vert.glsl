#version 430 core

layout(location = 0)in vec3 vertex;
layout(location = 1)in vec2 tex_coord;
layout(location = 2)in vec3 normal;

layout(location = 10)uniform mat4 MVP;
layout(location = 11)uniform mat4 MV;

layout(location = 13)uniform mat4 sunViewProjMatrix;
layout(location = 14)uniform mat4 spotViewProjMatrix;

out vec3 Position;
out vec3 Normal;
out vec2 texCoord;

out vec3 sunSpaceCoord;
out vec3 spotSpaceCoord;

void main(void) {
	Position = vec3( MV*vec4(vertex,1.0f) );
	Normal =	mat3(transpose(inverse(MV))) 
				//NormalMatrix
		* normal;

	vec4 ss = (sunViewProjMatrix*vec4(vertex.xyz, 1.0));
	sunSpaceCoord = (vec3(ss.xyz) / ss.w + 1) / 2;

	ss = (spotViewProjMatrix*vec4(vertex.xyz, 1.0));
	spotSpaceCoord = (vec3(ss.xyz) / ss.w + 1) / 2;

	gl_Position = MVP*vec4(vertex, 1.0f);
	texCoord = vec2(tex_coord.x, 1 - tex_coord.y);

}