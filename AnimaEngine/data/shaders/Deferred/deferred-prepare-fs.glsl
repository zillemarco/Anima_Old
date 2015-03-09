#version 150 core

in vec2 frag_textureCoord;
in vec3 frag_normal;
//in vec3 frag_tangent;
//in vec3 frag_bitangent;
in vec3 frag_worldPosition;
in mat3 frag_TBNMatrix;

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

out vec4 FragColor[4];

vec2 CalcParallaxTexCoords(vec3 worldPos, vec3 camPos, vec2 originalTextCoords, mat3 tbnMatrix)
{
	vec3 directionToEye = normalize(camPos - worldPos);	
	return originalTextCoords + (directionToEye * tbnMatrix).xy * (texture(MAT_DisplacementTexture, originalTextCoords).r * MAT_DisplacementScale + MAT_DisplacementBias);
}

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
	vec2 textureCoord = frag_textureCoord;

	vec4 color = MAT_DiffuseColor;
    vec4 textureColor = texture(MAT_DiffuseTexture, frag_textureCoord);	
	if(textureColor != vec4(0.0, 0.0, 0.0, 1.0))
		color *= textureColor;
		
	vec3 normal = calcNormal(frag_textureCoord);	
		
	FragColor[1] = color;
	FragColor[2] = vec4(normal * 0.5 + 0.5, 1.0);
	FragColor[3] = vec4(MAT_SpecularColor, 1.0 / MAT_Shininess);
}
