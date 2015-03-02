#version 410

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

uniform DirectionalLight _directionalLight;
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

vec4 calcDirectionalLight(DirectionalLight dLight, vec3 normal, vec3 worldPosition, vec3 materialSpecularColor, float materialShininess)
{
	return calcLight(dLight.base, dLight.direction, normal, worldPosition, materialSpecularColor, materialShininess);
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
	
	fragColor = color * calcDirectionalLight(_directionalLight, normal, worldPos, specularColor, shininess);
}
