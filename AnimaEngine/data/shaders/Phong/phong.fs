#version 410

const int MAX_POINT_LIGHTS = 2;
const int MAX_SPOT_LIGHTS = 2;

in vec2 frag_textureCoord;
in vec3 frag_normal;
in vec3 frag_worldPosition;
noperspective in vec3 frag_dist;

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

struct DirectionalLight
{
	BaseLight base;
	vec3 direction;
};

struct PointLight
{
	BaseLight base;
	Attenuation attenuation;
	vec3 position;
	float range;
};

struct SpotLight
{
	PointLight pointLight;
	vec3 direction;
	float cutoff;
};

uniform sampler2D _materialDiffuseTexture;
uniform vec4 _materialDiffuseColor;
uniform vec3 _materialSpecularColor;
uniform float _materialShininess;

uniform int _materialWireframe;
uniform vec4 _materialWireframeColor;

uniform vec3 _ambientLight;
uniform DirectionalLight _directionalLight;
uniform PointLight _pointLight[MAX_POINT_LIGHTS];
uniform SpotLight _spotLight[MAX_SPOT_LIGHTS];

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

vec4 calcDirectionalLight(DirectionalLight dLight, vec3 normal)
{
	return calcLight(dLight.base, dLight.direction, normal);
}

vec4 calcPointLight(PointLight pLight, vec3 normal)
{
	vec3 lightDirection = frag_worldPosition - pLight.position;
	float distanceToPoint = length(lightDirection);
	
	if(distanceToPoint > pLight.range)
		return vec4(0.0, 0.0, 0.0, 0.0);
	
	lightDirection = normalize(lightDirection);
		
	vec4 color = calcLight(pLight.base, lightDirection, normal);
	
	float att = pLight.attenuation.constant + 
				pLight.attenuation.linear * distanceToPoint + 
				pLight.attenuation.exponent * distanceToPoint * distanceToPoint +
				0.00001;
				
	return color / att;
}

vec4 calcSpotLight(SpotLight sLight, vec3 normal)
{
	vec3 lightDirection = normalize(frag_worldPosition - sLight.pointLight.position);
	float spotFactor = dot(lightDirection, sLight.direction);
	
	vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
	
	if(spotFactor > sLight.cutoff)
	{
		color = calcPointLight(sLight.pointLight, normal) *
				(1.0 - ((1.0 - spotFactor) / (1.0 - sLight.cutoff)));
	}
	
	return color;
}

void main()
{
	vec4 totalLight = vec4(_ambientLight, 1.0);
	vec4 color = _materialDiffuseColor;
    vec4 textureColor = texture(_materialDiffuseTexture, frag_textureCoord.xy);
	
	if(textureColor != vec4(0.0, 0.0, 0.0, 1.0))
		color *= textureColor;
		
	vec3 normal = normalize(frag_normal);
	
	totalLight += calcDirectionalLight(_directionalLight, normal);
	
	for(int i = 0; i < MAX_POINT_LIGHTS; i++)
		if(_pointLight[i].base.intensity > 0.0)
			totalLight += calcPointLight(_pointLight[i], normal);
			
	for(int i = 0; i < MAX_SPOT_LIGHTS; i++)
		if(_spotLight[i].pointLight.base.intensity > 0.0)
			totalLight += calcSpotLight(_spotLight[i], normal);
		
	//float nearD = min(min(frag_dist[0], frag_dist[1]), frag_dist[2]);	
	//float edgeIntensity = exp2(-1.0 * nearD * nearD);
	
	//if(_materialWireframe == 1)	
	//	fragColor = (edgeIntensity * _materialWireframeColor) + ((1.0 - edgeIntensity) * vec4(color * totalLight));
	//else
		fragColor = vec4(color * totalLight);
}
