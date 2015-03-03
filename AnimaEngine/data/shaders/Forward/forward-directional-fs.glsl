#version 410

in vec2 frag_textureCoord;
in vec3 frag_normal;
in vec3 frag_tangent;
in vec3 frag_bitangent;
in vec3 frag_worldPosition;

out vec4 fragColor;

struct BaseLight
{
	vec3 color;
	float intensity;
};

struct DirectionalLight
{
	BaseLight base;
	vec3 direction;
};

uniform sampler2D MAT_DiffuseTexture;
uniform sampler2D MAT_BumpTexture;
uniform sampler2D MAT_DisplacementTexture;
uniform vec4 MAT_DiffuseColor;
uniform bool MAT_HasBump;
uniform float MAT_DisplacementScale;
uniform float MAT_DisplacementBias;

uniform DirectionalLight _directionalLight;

uniform vec3 MAT_SpecularColor;
uniform float MAT_Shininess;
uniform vec3 CAM_Position;

vec4 calcLight(BaseLight base, vec3 direction, vec3 normal)
{
	float diffuseFactor = dot(normal, -direction);
	vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 specularColor = vec4(0.0, 0.0, 0.0, 0.0);
	
	if(diffuseFactor > 0.0)
	{
		diffuseColor = vec4(base.color, 1.0f) * base.intensity * diffuseFactor;
		
		vec3 directionToEye = normalize(CAM_Position - frag_worldPosition);
		vec3 reflectDirection = normalize(reflect(direction, normal));
		
		float specularFactor = dot(directionToEye, reflectDirection);
		specularFactor = pow(specularFactor, MAT_Shininess);
		
		if(specularFactor > 0.0)
		{
			specularColor = vec4(base.color, 1.0f) * vec4(MAT_SpecularColor, 1.0) * specularFactor;
		}
	}

	return diffuseColor + specularColor;
}

vec4 calcDirectionalLight(DirectionalLight dLight, vec3 normal)
{
	return calcLight(dLight.base, dLight.direction, normal);
}

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
	
	fragColor = color * calcDirectionalLight(_directionalLight, normal);
}
