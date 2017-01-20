#version 430 core

layout (location = 0) out vec4 fColor;
layout (location = 1) out vec4 blurColor;


in vec2 texCoord;

layout(location = 100) uniform sampler2D diffuse;
layout(location = 101) uniform vec3 m_specular;
layout(location = 102) uniform float shininess;

layout(location = 130) uniform sampler2DShadow sunShadowMap;
layout(location = 140) uniform sampler2DShadow spotShadowMap;

in vec3 sunSpaceCoord;
in vec3 spotSpaceCoord;


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

	float innerCut;
	float outerCut;

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


vec3 directLightCalculations(vec3 color);
vec3 pointLightCalculations(vec3 color);
vec3 spotLightCalculations(vec3 color);


void main(void) {
	vec4 COLOR = texture(diffuse, texCoord);
	vec3 color = vec3(COLOR.xyz);

	vec3 result;
	result = pointLightCalculations(color);
	result += directLightCalculations(color);
	result += spotLightCalculations(color);

	fColor = vec4( result, COLOR.a );
	blurColor = vec4(vec3(0), COLOR.a);

}

vec3 directLightCalculations(vec3 color) {
	vec3 result = vec3(0);
	int i = 0;
	if(numDirectLight>i)
	{

		vec3 normal = normalize(Normal);
		vec3 viewDir = normalize(-Position);
		vec3 lightDir = normalize( directLight[i].direction );

		vec3 halfwayDir = normalize(lightDir + viewDir);

		float diff = max(0.0, dot(lightDir, normal));
		float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

		vec3 ambient = directLight[i].ambient * color;
		vec3 diffuse = directLight[i].diffuse * diff * color;
		vec3 specular = directLight[i].specular * spec * m_specular*color;

		float alpha = texture(sunShadowMap, sunSpaceCoord);

		result += ambient*directLight[i].color + 
			alpha*
			diffuse*directLight[i].color + 
			alpha*
			specular*directLight[i].color;
	}

	for (i = 1; i < numDirectLight; i++)
	{
		vec3 normal = normalize(Normal);
		vec3 viewDir = normalize(-Position);
		vec3 lightDir = normalize( directLight[i].direction );

		vec3 halfwayDir = normalize(lightDir + viewDir);

		float diff = max(0.0, dot(lightDir, normal));
		float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

		vec3 ambient = directLight[i].ambient * color;
		vec3 diffuse = directLight[i].diffuse * diff * color;
		vec3 specular = directLight[i].specular * spec * m_specular*color;

		result += ambient*directLight[i].color + diffuse*directLight[i].color + specular*directLight[i].color;
	}
	return result;
}

vec3 pointLightCalculations(vec3 color) {
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


		vec3 scatteredLight = //ambient*vec3(pointLight[i].color.xyz) +
			vec3(pointLight[i].color.xyz)*diffuse*attenuation;

		vec3 reflectedLight = vec3(pointLight[i].color.xyz)*specular* attenuation;
		result += scatteredLight + reflectedLight;
	};

	return result;
}

vec3 spotLightCalculations(vec3 color) {

	vec3 result = vec3(0);
	int i = 0;
	if (numSpotLight > 0)
	{

		vec3 lightPosition = spotLight[i].position;

		float lightDistance = length(lightPosition - Position);

		vec3 normal = normalize(-Normal);
		vec3 viewDir = normalize(-Position);
		vec3 spotLightDir = normalize(
			//vec3(0,0,-1)
			spotLight[0].direction
		);
		vec3 lightDir = -normalize(lightPosition - Position);


		vec3 halfwayDir = normalize(lightDir + viewDir);

		float attenuation =
			1.0 / (spotLight[i].ConstantAttenuation +
				spotLight[i].LinearAttenuation * lightDistance + spotLight[i].QuadraticAttenuation * lightDistance * lightDistance);

		float diff = max(0.0, dot(lightDir, normal));
		float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

		vec3 ambient = spotLight[i].ambient * color;
		vec3 diffuse = spotLight[i].diffuse * diff * color;
		vec3 specular = spotLight[i].specular * spec * m_specular*color;

		float theta = dot(lightDir, spotLightDir);
		float intensity = smoothstep(spotLight[i].outerCut, spotLight[i].innerCut, theta);

		float alpha = texture(spotShadowMap, spotSpaceCoord);

		vec3 scatteredLight =
			//ambient*spotLight[i].color
			+spotLight[i].color*diffuse
			*attenuation
			*intensity
			*alpha
			;

		vec3 reflectedLight = spotLight[i].color*specular
			*attenuation
			*intensity
			*alpha
			;

		result += scatteredLight + reflectedLight;
	}
	for (int i = 1; i < numSpotLight; i++)
	{

		vec3 lightPosition = spotLight[i].position;

		float lightDistance = length(lightPosition - Position);

		vec3 normal = normalize(-Normal);
		vec3 viewDir = normalize(-Position);
		vec3 spotLightDir = normalize(
			//vec3(0,0,-1)
			spotLight[0].direction
		);
		vec3 lightDir = normalize(lightPosition - Position);


		vec3 halfwayDir = normalize(lightDir + viewDir);

		float attenuation =
			1.0 / (spotLight[i].ConstantAttenuation +
				spotLight[i].LinearAttenuation * lightDistance + spotLight[i].QuadraticAttenuation * lightDistance * lightDistance);

		float diff = max(0.0, dot(-lightDir, normal));
		float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

		vec3 ambient = spotLight[i].ambient * color;
		vec3 diffuse = spotLight[i].diffuse * diff * color;
		vec3 specular = spotLight[i].specular * spec * m_specular*color;

		float theta = dot(-lightDir, spotLightDir);
		float intensity = smoothstep(cos(spotLight[i].outerCut), cos(spotLight[i].innerCut), theta);

		vec3 scatteredLight = //ambient*spotLight[i].color*la 
			+spotLight[i].color*diffuse
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