#version 150 core

out vec4 FragColor[2];

uniform sampler2D REN_GB_PrepassBuffer_DepthMap;
uniform sampler2D REN_GB_PrepassBuffer_NormalMap;
uniform sampler2D REN_GB_PrepassBuffer_SpecularMap;
uniform vec2 REN_InverseScreenSize;
uniform vec3 CAM_Position;
uniform mat4 CAM_ProjectionViewInverseMatrix;

uniform float DIL_Range;
uniform vec3 DIL_Direction;
uniform vec3 DIL_Color;
uniform float DIL_Intensity;
uniform sampler2D DIL_ShadowMap;
uniform float DIL_ShadowMapBias;
uniform vec2 DIL_ShadowMapTexelSize;
uniform mat4 DIL_ProjectionViewMatrix;

float lineStep(float low, float hight, float compare)
{
	return clamp((compare - low) / (hight - low), 0.0f, 1.0f);
}

float ComputeShadowAmount(sampler2D shadowMap, vec2 coords, float compare)
{
	//VAR vec2 moments 	= texture(shadowMap, coords).xy;
	//VAR float p 		= step(compare, moments.x);
	//VAR float variance 	= max(moments.y - moments.x * moments.x, 0.0000002);

	//VAR float d 		= compare - moments.x;
	//VAR float pMax 		= variance / (variance + d * d);
	//VAR pMax 			= lineStep(0.02f, 1.0f, pMax);

	//VAR return pMax;

	vec2 pixelPos = coords / DIL_ShadowMapTexelSize + vec2(0.5f);
	vec2 fractPart = fract(pixelPos);
	vec2 startTexel = (pixelPos - fractPart) * DIL_ShadowMapTexelSize;

	float bl = step(compare, texture(shadowMap, startTexel).r + DIL_ShadowMapBias);
	float br = step(compare, texture(shadowMap, startTexel + vec2(DIL_ShadowMapTexelSize.x, 0.0f)).r + DIL_ShadowMapBias);
	float tl = step(compare, texture(shadowMap, startTexel + vec2(0.0f, DIL_ShadowMapTexelSize.t)).r + DIL_ShadowMapBias);
	float tr = step(compare, texture(shadowMap, startTexel + DIL_ShadowMapTexelSize).r + DIL_ShadowMapBias);

	float mixA = mix(bl, tl, fractPart.y);
	float mixB = mix(br, tr, fractPart.y);

	return mix(mixA, mixB, fractPart.x);
}

void main()
{
	vec3 genPos	= vec3((gl_FragCoord.x * REN_InverseScreenSize.x), (gl_FragCoord.y * REN_InverseScreenSize.y), 0.0f);
	genPos.z 	= texture(REN_GB_PrepassBuffer_DepthMap, genPos.xy).r;

	vec3 normal 		= normalize(texture(REN_GB_PrepassBuffer_NormalMap, genPos.xy).xyz * 2.0f - 1.0f);
	vec4 speclarData 	= texture(REN_GB_PrepassBuffer_SpecularMap, genPos.xy);
	vec4 clip 			= CAM_ProjectionViewInverseMatrix * vec4(genPos * 2.0f - 1.0f, 1.0f);
	vec3 pos 			= clip.xyz / clip.w;

	vec4 shadowCoord 	= DIL_ProjectionViewMatrix * vec4(pos, 1.0f);
	shadowCoord 		/= shadowCoord.w;
	shadowCoord.xyz		= shadowCoord.xyz * vec3(0.5f, 0.5f, 0.5f) + vec3(0.5f, 0.5f, 0.5f);

	float shadowAmount 	= 1.0f;//ComputeShadowAmount(DIL_ShadowMap, shadowCoord.xy, shadowCoord.z);

	vec3 specularColor 	= speclarData.xyz;
	float shininess 	= 1.0f / speclarData.a;

	vec3 incident 	= normalize(-DIL_Direction);
	vec3 viewDir 	= normalize(CAM_Position - pos);
	vec3 halfDir 	= normalize(incident + viewDir);

	float lambert 	= clamp(dot(incident, normal), 0.0f, 1.0f);
	float rFactor 	= clamp(dot(halfDir, normal), 0.0f, 1.0f);
	float sFactor 	= pow(rFactor, shininess);
	vec3 sColor 	= specularColor * sFactor;

	FragColor[0] = vec4(DIL_Color * lambert * shadowAmount * DIL_Intensity, 1.0f);
	FragColor[1] = vec4(DIL_Color * sColor * shadowAmount * DIL_Intensity, 1.0f);
}
