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
uniform sampler2D LIG_ShadowMap;
uniform mat4 LIG_ProjectionViewMatrix;

void main()
{
	vec3 genPos	= vec3((gl_FragCoord.x * REN_InverseScreenSize.x), (gl_FragCoord.y * REN_InverseScreenSize.y), 0.0f);
	genPos.z 	= texture(REN_GB_PrepassBuffer_DepthMap, genPos.xy).r;

	vec3 normal 		= normalize(texture(REN_GB_PrepassBuffer_NormalMap, genPos.xy).xyz * 2.0f - 1.0f);
	vec4 speclarData 	= texture(REN_GB_PrepassBuffer_SpecularMap, genPos.xy);
	vec4 clip 			= CAM_ProjectionViewInverseMatrix * vec4(genPos * 2.0f - 1.0f, 1.0f);
	vec3 pos 			= clip.xyz / clip.w;

	vec4 shadowCoord 	= LIG_ProjectionViewMatrix * vec4(genPos * 2.0f - 1.0f, 1.0f);
	shadowCoord 		/= shadowCoord.w;
	shadowCoord.xyz		= shadowCoord.xyz * vec3(0.5f, 0.5f, 0.5f) + vec3(0.5f, 0.5f, 0.5f);

	float shadowAmount 	= step(shadowCoord.z, texture(LIG_ShadowMap, shadowCoord.xy).r);

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
