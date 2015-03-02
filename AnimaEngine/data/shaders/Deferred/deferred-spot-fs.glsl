#version 410

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

struct SpotLight
{
	PointLight pointLight;
	vec3 direction;
	float cutoff;
};

uniform SpotLight _spotLight;
uniform vec3 _cameraPosition;

uniform vec2 _renderingScreenSize;
uniform sampler2D _renderingDeferredAlbedoMap;
uniform sampler2D _renderingDeferredNormalMap;
uniform sampler2D _renderingDeferredWorldPosMap;
uniform sampler2D _renderingDeferredSpecularMap;

vec4 calcLight(BaseLight base, vec3 direction, vec3 normal, vec3 worldPosition, vec3 materialSpecularColor, float materialShininess)
{
	float diffuseFactor = dot(normal, -direction);
	vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 specularColor = vec4(0.0, 0.0, 0.0, 0.0);
	
	if(diffuseFactor > 0.0)
	{
		diffuseColor = vec4(base.color, 1.0f) * base.intensity * diffuseFactor;
		
		vec3 directionToEye = normalize(_cameraPosition - worldPosition);
		vec3 reflectDirection = normalize(reflect(direction, normal));
		
		float specularFactor = dot(directionToEye, reflectDirection);
		specularFactor = pow(specularFactor, materialShininess);
		
		if(specularFactor > 0.0)
		{
			specularColor = vec4(base.color, 1.0f) * vec4(materialSpecularColor, 1.0) * specularFactor;
		}
	}

	return diffuseColor + specularColor;
}

vec4 calcPointLight(PointLight pLight, vec3 normal, vec3 worldPosition, vec3 materialSpecularColor, float materialShininess)
{
	vec3 lightDirection = worldPosition - pLight.position;
	float distanceToPoint = length(lightDirection);
	
	lightDirection = normalize(lightDirection);
		
	vec4 color = calcLight(pLight.base, lightDirection, normal, worldPosition, materialSpecularColor, materialShininess);
	
	float att = pLight.attenuation.constant + 
				pLight.attenuation.linear * distanceToPoint + 
				pLight.attenuation.exponent * distanceToPoint * distanceToPoint +
				0.00001;
				
	return color / att;
}

vec4 calcSpotLight(SpotLight sLight, vec3 normal, vec3 worldPosition, vec3 materialSpecularColor, float materialShininess)
{
	vec3 lightDirection = normalize(worldPosition - sLight.pointLight.position);
	float spotFactor = dot(lightDirection, sLight.direction);
	
	vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
	
	if(spotFactor > sLight.cutoff)
	{
		color = calcPointLight(sLight.pointLight, normal, worldPosition, materialSpecularColor, materialShininess) *
				(1.0 - ((1.0 - spotFactor) / (1.0 - sLight.cutoff)));
	}
	
	return color;
}

vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / _renderingScreenSize;
}

void main()
{
	vec2 textCoord = CalcTexCoord();
	vec4 color = texture(_renderingDeferredAlbedoMap, textCoord);
	vec3 normal = texture(_renderingDeferredNormalMap, textCoord).xyz;
	vec3 worldPos = texture(_renderingDeferredWorldPosMap, textCoord).xyz;
	vec3 specularColor = texture(_renderingDeferredSpecularMap, textCoord).xyz;
	float shininess = texture(_renderingDeferredSpecularMap, textCoord).w;
	
	fragColor = color * calcSpotLight(_spotLight, normal, worldPos, specularColor, shininess);
}
