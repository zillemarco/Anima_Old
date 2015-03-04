#version 150 core

in mat4 frag_inverseProjectionViewMatrix;
out vec4 FragColor[2];

uniform sampler2D REN_GB_PrepassBuffer_DepthMap;
uniform sampler2D REN_GB_PrepassBuffer_NormalMap;
uniform vec2 REN_InverseScreenSize;
uniform vec3 CAM_Position;

uniform float PTL_Range;
uniform vec3 PTL_Position;
uniform vec3 PTL_Color;
uniform float PTL_ConstantAttenuation;
uniform float PTL_LinearAttenuation;
uniform float PTL_ExponentAttenuation;

void main()
{
	vec3 pos 	= vec3((gl_FragCoord.x * REN_InverseScreenSize.x), (gl_FragCoord.y * REN_InverseScreenSize.y), 0.0f);
	pos.z 		= texture(REN_GB_PrepassBuffer_DepthMap, pos.xy).r;

	vec3 normal = normalize(texture(REN_GB_PrepassBuffer_NormalMap, pos.xy).xyz * 2.0f - 1.0f);
	vec4 clip 	= frag_inverseProjectionViewMatrix * vec4(pos * 2.0f - 1.0f, 1.0f);
	pos 		= clip.xyz / clip.w;

	float dist 	= length(PTL_Position - pos);

	if(dist > PTL_Range)
	{
		discard;
	}

	float atten = (PTL_ConstantAttenuation + PTL_LinearAttenuation * dist +  PTL_ExponentAttenuation * dist * dist + 0.00001);

	vec3 incident 	= normalize(PTL_Position - pos);
	vec3 viewDir 	= normalize(CAM_Position - pos);
	vec3 halfDir 	= normalize(incident + viewDir);

	float lambert 	= clamp(dot(incident, normal), 0.0f, 1.0f);
	float rFactor 	= clamp(dot(halfDir, normal), 0.0f, 1.0f);
	float sFactor 	= pow(rFactor, 33.0f);

	FragColor[0] = vec4(PTL_Color * lambert / atten, 1.0f);
	FragColor[1] = vec4(PTL_Color * sFactor / atten * 0.33f, 1.0f);
}
