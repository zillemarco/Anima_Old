#version 410

in vec2 frag_textureCoord;
in vec3 frag_normal;
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

uniform sampler2D materialDiffuseTexture;
uniform vec4 materialColor;

uniform PointLight pointLight;

uniform float specularIntensity;
uniform float specularPower;
uniform vec3 eyePosition;

vec4 calcLight(BaseLight base, vec3 direction, vec3 normal)
{
	float diffuseFactor = dot(normal, -direction);
	vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 specularColor = vec4(0.0, 0.0, 0.0, 0.0);
	
	if(diffuseFactor > 0.0)
	{
		diffuseColor = vec4(base.color, 1.0f) * base.intensity * diffuseFactor;
		
		vec3 directionToEye = normalize(eyePosition - frag_worldPosition);
		vec3 reflectDirection = normalize(reflect(direction, normal));
		
		float specularFactor = dot(directionToEye, reflectDirection);
		specularFactor = pow(specularFactor, specularPower);
		
		if(specularFactor > 0.0)
		{
			specularColor = vec4(base.color, 1.0f) * specularIntensity * specularFactor;
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

void main()
{
	vec4 color = materialColor;
    vec4 textureColor = texture(materialDiffuseTexture, frag_textureCoord.xy);
	
	if(textureColor != vec4(0.0, 0.0, 0.0, 1.0))
		color *= textureColor;
		
	vec3 normal = normalize(frag_normal);
	
	fragColor = color * calcPointLight(pointLight, normal);
}
