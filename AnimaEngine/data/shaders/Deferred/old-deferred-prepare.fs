#version 410

layout (location = 1) out vec4 albedoMap;
layout (location = 2) out vec4 normalMap;

in vec2 frag_textureCoord;
in vec3 frag_normal;
in vec3 frag_tangent;
in vec3 frag_bitangent;
in vec3 frag_worldPosition;

uniform sampler2D MAT_DiffuseTexture;
uniform vec4 MAT_DiffuseColor;

uniform sampler2D MAT_BumpTexture;
uniform bool MAT_HasBump;

uniform sampler2D MAT_DisplacementTexture;
uniform float MAT_DisplacementScale;
uniform float MAT_DisplacementBias;

uniform vec3 MAT_SpecularColor;
uniform float MAT_Shininess;

uniform vec3 CAM_Position;

vec2 CalcParallaxTexCoords()
{
	vec3 directionToEye = normalize(CAM_Position - frag_worldPosition);
	vec3 normal = normalize(frag_normal);
	vec3 tangent = normalize(frag_tangent);
	vec3 bitangent = normalize(frag_bitangent);
	mat3 TBN = mat3(tangent, bitangent, normal);
	
	return frag_textureCoord.xy + (directionToEye * TBN).xy * (texture(MAT_DisplacementTexture, frag_textureCoord.xy).r * MAT_DisplacementScale + MAT_DisplacementBias);
}

vec3 calcNormal()
{
	vec3 normal = normalize(frag_normal);
	
	if(!MAT_HasBump)
		return normal;
		
	vec3 tangent = normalize(frag_tangent);
	vec3 bitangent = normalize(frag_bitangent);
	
	vec3 bumpNormal = texture(MAT_BumpTexture, CalcParallaxTexCoords().xy).xyz;
	bumpNormal = 2.0 * bumpNormal - vec3(1.0, 1.0, 1.0);
	
	mat3 TBN = mat3(tangent, bitangent, normal);
	vec3 newNormal = TBN * bumpNormal;
	newNormal = normalize(newNormal);
	
	return newNormal;
}

void main()
{
	vec4 color = MAT_DiffuseColor;
    vec4 textureColor = texture(MAT_DiffuseTexture, CalcParallaxTexCoords().xy);	
	if(textureColor != vec4(0.0, 0.0, 0.0, 1.0))
		color *= textureColor;
		
	vec3 normal = calcNormal();	
		
	albedoMap = color;
	normalMap = vec4(normal * 0.5 + 0.5, 1.0);
}
