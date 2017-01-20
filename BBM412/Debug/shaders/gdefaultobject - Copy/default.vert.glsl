#version 430 core

layout(location = 0)in vec3 vertex;
layout(location = 1)in vec2 tex_coord;
layout(location = 2)in vec3 normal;

layout(location = 10)uniform mat4 MVP;
layout(location = 11)uniform mat4 MV;
layout(location = 12)uniform mat3 NormalMatrix;
//layout(location = 13)uniform mat4 sunViewProjMatrix;

out vec3 Position;
out vec3 Normal;
out vec2 texCoord;
out vec3 sunSpaceCoord;

void main(void) {
	Position = vec3( MV*vec4(vertex,1.0f) );
	Normal =	mat3(transpose(inverse(MV))) 
				//NormalMatrix
		* normal;

	//sunSpaceCoord = vec3( (sunViewProjMatrix*(vertex, 1.0) ) );
					//*Position.xyz/Position.w;

	gl_Position = MVP*vec4(vertex, 1.0f);
	texCoord = vec2(tex_coord.x, 1 - tex_coord.y);

}