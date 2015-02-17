#version 410

in vec2 frag_textureCoord;
in vec3 frag_normal;
in vec3 frag_tangent;
in vec3 frag_bitangent;
in vec3 frag_worldPosition;

out vec4 fragColor;

struct Attenuation
{
	float constant;
	float linear;
	float exponent;
};

struct BaseLight
{
	vec3 color;
	float intensity;
};

struct PointLight
{
	BaseLight base;
	Attenuation attenuation;
	vec3 position;
};

uniform sampler2D _materialDiffuseTexture;
uniform sampler2D _materialBumpTexture;
uniform sampler2D _materialDisplacementTexture;
uniform vec4 _materialDiffuseColor;
uniform bool _materialHasBump;
uniform float _materialDisplacementScale;
uniform float _materialDisplacementBias;

uniform PointLight _pointLight;

uniform vec3 _materialSpecularColor;
uniform float _materialShininess;
uniform vec3 _cameraPosition;

vec4 calcLight(BaseLight base, vec3 direction, vec3 normal)
{
	float diffuseFactor = dot(normal, -direction);
	vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 specularColor = vec4(0.0, 0.0, 0.0, 0.0);
	
	if(diffuseFactor > 0.0)
	{
		diffuseColor = vec4(base.color, 1.0f) * base.intensity * diffuseFactor;
		
		vec3 directionToEye = normalize(_cameraPosition - frag_worldPosition);
		vec3 reflectDirection = normalize(reflect(direction, normal));
		
		float specularFactor = dot(directionToEye, reflectDirection);
		specularFactor = pow(specularFactor, _materialShininess);
		
		if(specularFactor > 0.0)
		{
			specularColor = vec4(base.color, 1.0f) * vec4(_materialSpecularColor, 1.0) * specularFactor;
		}
	}

	return diffuseColor + specularColor;
}

vec4 calcPointLight(PointLight pLight, vec3 normal)
{
	vec3 lightDirection = frag_worldPosition - pLight.position;
	float distanceToPoint = length(lightDirection);
	
	lightDirection = normalize(lightDirection);
		
	vec4 color = calcLight(pLight.base, lightDirection, normal);
	
	float att = pLight.attenuation.constant + 
				pLight.attenuation.linear * distanceToPoint + 
				pLight.attenuation.exponent * distanceToPoint * distanceToPoint +
				0.00001;
				
	return color / att;
}

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
	
	fragColor = color * calcPointLight(_pointLight, normal);
}
