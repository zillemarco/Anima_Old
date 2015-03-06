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

void main()
{
	vec3 pos 	= vec3((gl_FragCoord.x * REN_InverseScreenSize.x), (gl_FragCoord.y * REN_InverseScreenSize.y), 0.0f);
	pos.z 		= texture(REN_GB_PrepassBuffer_DepthMap, pos.xy).r;

	vec3 normal 		= normalize(texture(REN_GB_PrepassBuffer_NormalMap, pos.xy).xyz * 2.0f - 1.0f);
	vec4 speclarData 	= texture(REN_GB_PrepassBuffer_SpecularMap, pos.xy);
	vec4 clip 			= CAM_ProjectionViewInverseMatrix * vec4(pos * 2.0f - 1.0f, 1.0f);
	pos 				= clip.xyz / clip.w;

	vec3 specularColor 	= speclarData.xyz;
	float shininess 	= 1.0f / speclarData.a;

	vec3 incident 	= normalize(-DIL_Direction);
	vec3 viewDir 	= normalize(CAM_Position - pos);
	vec3 halfDir 	= normalize(incident + viewDir);

	float lambert 	= clamp(dot(incident, normal), 0.0f, 1.0f);
	float rFactor 	= clamp(dot(halfDir, normal), 0.0f, 1.0f);
	float sFactor 	= pow(rFactor, shininess);
	vec3 sColor 	= specularColor * sFactor;

	FragColor[0] = vec4(DIL_Color * lambert * DIL_Intensity, 1.0f);
	FragColor[1] = vec4(DIL_Color * sColor * DIL_Intensity, 1.0f);
}
