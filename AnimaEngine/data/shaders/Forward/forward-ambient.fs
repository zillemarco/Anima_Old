#version 410

in vec2 frag_textureCoord;
in vec3 frag_normal;
in vec3 frag_tangent;
in vec3 frag_bitangent;
in vec3 frag_worldPosition;

out vec4 fragColor;

uniform sampler2D _materialDiffuseTexture;
uniform sampler2D _materialDisplacementTexture;
uniform vec4 _materialDiffuseColor;
uniform float _materialDisplacementScale;
uniform float _materialDisplacementBias;
uniform vec3 _cameraPosition;

uniform vec3 _ambientLight;

vec2 CalcParallaxTexCoords()
{
	vec3 directionToEye = normalize(_cameraPosition - frag_worldPosition);
	vec3 normal = normalize(frag_normal);
	vec3 tangent = normalize(frag_tangent);
	vec3 bitangent = normalize(frag_bitangent);
	mat3 TBN = mat3(tangent, bitangent, normal);
	
	return frag_textureCoord.xy + (directionToEye * TBN).xy * (texture(_materialDisplacementTexture, frag_textureCoord.xy).r * _materialDisplacementScale + _materialDisplacementBias);
}

void main()
{
	vec4 color = _materialDiffuseColor;
    vec4 textureColor = texture(_materialDiffuseTexture, CalcParallaxTexCoords().xy);
	
	if(textureColor != vec4(0.0, 0.0, 0.0, 1.0))
		color *= textureColor;
		
	fragColor = color * vec4(_ambientLight, 1.0);
}
