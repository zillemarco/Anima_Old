#version 410

layout (location = 1) out vec4 albedoMap;
layout (location = 2) out vec4 normalMap;

in vec2 frag_textureCoord;
in vec3 frag_normal;
in vec3 frag_tangent;
in vec3 frag_bitangent;
in vec3 frag_worldPosition;
in mat3 frag_TBNMatrix;

uniform sampler2D MAT_DiffuseTexture;
uniform vec4 MAT_DiffuseColor;

uniform sampler2D MAT_BumpTexture;
uniform bool MAT_HasBump;

uniform vec3 MAT_SpecularColor;
uniform float MAT_Shininess;

vec3 calcNormal(vec2 textureCoords)
{
	if(!MAT_HasBump)
		return frag_normal;
	
	vec3 bumpNormal = texture(MAT_BumpTexture, textureCoords).xyz;
	bumpNormal = 2.0 * bumpNormal - vec3(1.0, 1.0, 1.0);	
	vec3 newNormal = frag_TBNMatrix * bumpNormal;
	newNormal = normalize(newNormal);
	
	return newNormal;
}

void main()
{
	vec4 color = MAT_DiffuseColor;
    vec4 textureColor = texture(MAT_DiffuseTexture, frag_textureCoord);	
	if(textureColor != vec4(0.0, 0.0, 0.0, 1.0))
		color *= textureColor;
		
	vec3 normal = calcNormal(frag_textureCoord);	
		
	albedoMap = color;
	normalMap = vec4(normal * 0.5 + 0.5, 1.0);
}
