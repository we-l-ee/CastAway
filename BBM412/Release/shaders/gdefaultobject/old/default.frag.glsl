#version 430 core

in vec2 texCoord;

layout(location = 100) uniform sampler2D diffuse;
layout(location = 101) uniform vec3 m_specular;
layout(location = 102) uniform float shininess;


layout(location = 13)uniform mat4 ViewMatrix;

in vec3 Position;
in vec3 Normal;
/*

*/
struct DirectLight_t {

	vec3 direction;
	vec3 color;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};
struct PointLight_t {

	vec3 position;
	vec3 color;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float ConstantAttenuation;
	float LinearAttenuation;
	float QuadraticAttenuation;

};

struct SpotLight_t {

	vec3 position;
	vec3 direction;
	vec3 color;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float cutOff;
	float outerCutOff;

	float ConstantAttenuation;
	float LinearAttenuation;
	float QuadraticAttenuation;

};

layout(std140, binding = 0) uniform DirectLight{
	DirectLight_t directLight[5];
};
layout(location = 120) uniform int numDirectLight;

layout(std140, binding=1) uniform PointLight{
	PointLight_t pointLight[10];
};
layout(location = 121)uniform int numPointLight;

layout(std140, binding = 2) uniform SpotLight{
	SpotLight_t spotLight[10];
};
layout(location = 122)uniform int numSpotLight;


out vec4 fColor;

vec3 directLightCalculations();
vec3 pointLightCalculations();
vec3 spotLightCalculations();


void main(void) {
	/*
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(-Position);
	vec3 lightDir = normalize(directLight[0].direction);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float diff = max(0.0, dot(lightDir, normal));
	*/

	/*
	vec3 lightPosition = pointLight[0].position;
	float lightDistance = length(lightPosition - Position);
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(-Position);
	vec3 lightDir = normalize(lightPosition - Position);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float diff = max(0.0, dot(lightDir, normal));
	float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

	float attenuation =
		1.0 / (pointLight[0].ConstantAttenuation +
			pointLight[0].LinearAttenuation * lightDistance + pointLight[0].QuadraticAttenuation * lightDistance * lightDistance);
	*/

	vec3 result;
	result = pointLightCalculations();
	result += directLightCalculations();
	result += spotLightCalculations();

	
	vec3 lightPosition = spotLight[0].position;

	float lightDistance = length(lightPosition - Position);
	float attenuation =
		1.0 / (spotLight[0].ConstantAttenuation +
			spotLight[0].LinearAttenuation * lightDistance + spotLight[0].QuadraticAttenuation * lightDistance * lightDistance);

	fColor = vec4(
		//normalize(pointLight[0].position)
		//ambient*attenuation*2 + 
		//diff*lightColor*attenuation*5
		//pointLight[0].specular
		//material.diffuse
		result
		, 1.0)
		//*attenuation;
		;
		//pow(texture(tex, texCoord),vec4(1.0/ 2.2));

	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(-Position);
	vec3 lightDir = normalize(spotLight[0].direction);

	float diff = max(0.0, dot(lightDir, normal));

	vec3 halfwayDir = normalize(lightDir + viewDir);

	//fColor = texture(diffuse, texCoord)
		//*spotLight[0].LinearAttenuation;
		//*spotLight[0].ConstantAttenuation;
		//*attenuation;
		//*lightDistance;
		//*diff*attenuation;
		//*attenuation;
		//*pointLight[0].ConstantAttenuation;
		//*pointLight[0].ConstantAttenuation;
		//*1/(1 + pointLight[0].LinearAttenuation*lightDistance );
		//*pointLight[0].LinearAttenuation;
		//*attenuation;
		//*vec4(lightDir.xyz,1.0);
		//*vec4(pointLight[0].diffuse, 1.0);
		//*vec4(directLight[0].color, 1.0);

}

vec3 directLightCalculations() {
	vec3 result = vec3(0);
	for (int i = 0; i < numDirectLight; i++)
	{
		vec3 normal = normalize(Normal);
		vec3 viewDir = normalize(-Position);
		vec3 lightDir = normalize( directLight[i].direction );

		vec3 halfwayDir = normalize(lightDir + viewDir);

		float diff = max(0.0, dot(lightDir, normal));
		float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

		vec3 color = vec3(texture(diffuse, texCoord).xyz);

		vec3 ambient = directLight[i].ambient * color;
		vec3 diffuse = directLight[i].diffuse * diff * color;
		vec3 specular = directLight[i].specular * spec * (color*m_specular);

		result += ambient*directLight[i].color + diffuse*directLight[i].color + specular*directLight[i].color;
	}
	return result;
}

vec3 pointLightCalculations() {
	vec3 result = vec3(0);
	for (int i = 0; i < numPointLight; i++) {

		vec3 lightPosition = pointLight[i].position;
		float lightDistance = length(lightPosition - Position);

		vec3 normal = normalize(Normal);
		vec3 viewDir = normalize(-Position);
		vec3 lightDir = normalize(lightPosition - Position);


		vec3 halfwayDir = normalize(lightDir + viewDir);

		float attenuation =
			1.0 / (pointLight[i].ConstantAttenuation +
				pointLight[i].LinearAttenuation * lightDistance + pointLight[i].QuadraticAttenuation * lightDistance * lightDistance);

		float diff = max(0.0, dot(lightDir, normal));
		float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

		vec3 ambient = pointLight[i].ambient * vec3(texture(diffuse, texCoord).xyz);
		vec3 diffuse = pointLight[i].diffuse * diff * vec3(texture(diffuse, texCoord).xyz);
		vec3 specular = pointLight[i].specular * spec * m_specular;


		vec3 scatteredLight =ambient*vec3(pointLight[i].color.xyz) +
			vec3(pointLight[i].color.xyz)*diffuse*attenuation;

		vec3 reflectedLight = vec3(pointLight[i].color.xyz)*specular* attenuation;
		result += scatteredLight + reflectedLight;
	};

	return result;
}

vec3 spotLightCalculations() {

	vec3 result = vec3(0);
	for (int i = 0; i < numSpotLight; i++)
	{
			float outerCut = cos(radians(15));
			float innerCut = cos(radians(12));
		
		vec3 lightPosition = spotLight[i].position;

		float lightDistance = length(lightPosition - Position);

		vec3 normal = normalize(-Normal);
		vec3 viewDir = normalize(-Position);
		vec3 spotLightDir = normalize(
			//vec3(0,0,-1)
			spotLight[0].direction
		);
		vec3 lightDir = normalize(lightPosition - Position );


		vec3 halfwayDir = normalize(lightDir + viewDir);

		float attenuation =
			1.0 / (spotLight[i].ConstantAttenuation +
				spotLight[i].LinearAttenuation * lightDistance + spotLight[i].QuadraticAttenuation * lightDistance * lightDistance);

		float diff = max(0.0, dot(-lightDir, normal));
		float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

		vec3 ambient = spotLight[i].ambient * vec3( texture(diffuse, texCoord).rgb );
		vec3 diffuse = spotLight[i].diffuse * diff * vec3(texture(diffuse, texCoord).rgb);
		vec3 specular = spotLight[i].specular * spec * m_specular;

		float theta = dot(-lightDir, spotLightDir);
		float epsilon = innerCut - outerCut;
		float intensity = clamp(
			(theta) / epsilon
			//;
			, 0.0, 1.0);
		intensity = smoothstep(outerCut, innerCut, theta);
		float la = 1.0 / (spotLight[i].ConstantAttenuation +
			spotLight[i].LinearAttenuation * lightDistance);

		vec3 scatteredLight = ambient*spotLight[i].color*la + spotLight[i].color*diffuse
			*attenuation
			*intensity
			;

		vec3 reflectedLight = spotLight[i].color*specular
			*attenuation
			*intensity
			;

		result += scatteredLight + reflectedLight;


	};

	return result;


}