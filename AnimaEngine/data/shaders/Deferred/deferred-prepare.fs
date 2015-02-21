#version 410

layout (location = 1) out vec4 albedoMap;
layout (location = 2) out vec4 normalMap;

in vec2 frag_textureCoord;
in vec3 frag_normal;
in vec3 frag_tangent;
in vec3 frag_bitangent;
in vec3 frag_worldPosition;

uniform sampler2D _materialDiffuseTexture;
uniform vec4 _materialDiffuseColor;

uniform sampler2D _materialBumpTexture;
uniform bool _materialHasBump;

uniform sampler2D _materialDisplacementTexture;
uniform float _materialDisplacementScale;
uniform float _materialDisplacementBias;

uniform vec3 _materialSpecularColor;
uniform float _materialShininess;

uniform vec3 _cameraPosition;

vec2 CalcParallaxTexCoords()
{
	vec3 directionToEye = normalize(_cameraPosition - frag_worldPosition);
	vec3 normal = normalize(frag_normal);
	vec3 tangent = normalize(frag_tangent);
	vec3 bitangent = normalize(frag_bitangent);
	mat3 TBN = mat3(tangent, bitangent, normal);
	
	return frag_textureCoord.xy + (directionToEye * TBN).xy * (texture(_materialDisplacementTexture, frag_textureCoord.xy).r * _materialDisplacementScale + _materialDisplacementBias);
}

vec3 calcNormal()
{
	vec3 normal = normalize(frag_normal);
	
	if(!_materialHasBump)
		return normal;
		
	vec3 tangent = normalize(frag_tangent);
	vec3 bitangent = normalize(frag_bitangent);
	
	vec3 bumpNormal = texture(_materialBumpTexture, CalcParallaxTexCoords().xy).xyz;
	bumpNormal = 2.0 * bumpNormal - vec3(1.0, 1.0, 1.0);
	
	mat3 TBN = mat3(tangent, bitangent, normal);
	vec3 newNormal = TBN * bumpNormal;
	newNormal = normalize(newNormal);
	
	return newNormal;
}

void main()
{
	vec4 color = _materialDiffuseColor;
    vec4 textureColor = texture(_materialDiffuseTexture, CalcParallaxTexCoords().xy);	
	if(textureColor != vec4(0.0, 0.0, 0.0, 1.0))
		color *= textureColor;
		
	vec3 normal = calcNormal();	
		
	albedoMap = color;
	normalMap = vec4(normal * 0.5 + 0.5, 1.0);
}
