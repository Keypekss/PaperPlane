#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

struct Material {
	vec4 diffuse;
	vec4 specular;
	vec4 emissive;
	vec4 transparent;
	vec4 reflective;
	float shininess;
};

uniform int noTex;

uniform Material material;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

void main()
{
	vec4 texDiff;
	vec4 texSpec;
	vec4 texEmiss;
	vec4 texTransp;
	vec4 texReflect;
	vec4 result;

	if (noTex == 1) {
		texDiff = material.diffuse;
		texSpec = material.specular;
		texEmiss = material.emissive;
		texTransp = material.transparent;
		texReflect = material.reflective;
		result = texDiff * texSpec * texTransp * texReflect;
	} else {
		texDiff = texture(diffuse0, TexCoords);
		texSpec = texture(specular0, TexCoords);
		result = texDiff * texSpec;
	}
	
	FragColor = result;
}
