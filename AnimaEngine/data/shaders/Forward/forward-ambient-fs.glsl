#version 410

in vec2 frag_textureCoord;
in vec3 frag_normal;
in vec3 frag_tangent;
in vec3 frag_bitangent;
in vec3 frag_worldPosition;

out vec4 fragColor;

uniform sampler2D MAT_DiffuseTexture;
uniform sampler2D MAT_DisplacementTexture;
uniform vec4 MAT_DiffuseColor;
uniform float MAT_DisplacementScale;
uniform float MAT_DisplacementBias;
uniform vec3 CAM_Position;

uniform vec3 _ambientLight;

vec2 CalcParallaxTexCoords()
{
	vec3 directionToEye = normalize(CAM_Position - frag_worldPosition);
	vec3 normal = normalize(frag_normal);
	vec3 tangent = normalize(frag_tangent);
	vec3 bitangent = normalize(frag_bitangent);
	mat3 TBN = mat3(tangent, bitangent, normal);
	
	return frag_textureCoord.xy + (directionToEye * TBN).xy * (texture(MAT_DisplacementTexture, frag_textureCoord.xy).r * MAT_DisplacementScale + MAT_DisplacementBias);
}

void main()
{
	vec4 color = MAT_DiffuseColor;
    vec4 textureColor = texture(MAT_DiffuseTexture, CalcParallaxTexCoords().xy);
	
	if(textureColor != vec4(0.0, 0.0, 0.0, 1.0))
		color *= textureColor;
		
	fragColor = color * vec4(_ambientLight, 1.0);
}
