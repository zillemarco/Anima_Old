#version 150 core

in mat4 frag_inverseProjectionViewMatrix;
out vec4 FragColor[2];

uniform sampler2D REN_GB_PrepassBuffer_DepthMap;
uniform sampler2D REN_GB_PrepassBuffer_NormalMap;
uniform vec2 REN_InverseScreenSize;

uniform vec3 HEL_Position;
uniform vec3 HEL_SkyColor;
uniform vec3 HEL_GroundColor;
uniform float HEL_Intensity;

void main()
{
	vec3 pos 	= vec3((gl_FragCoord.x * REN_InverseScreenSize.x), (gl_FragCoord.y * REN_InverseScreenSize.y), 0.0f);
	pos.z 		= texture(REN_GB_PrepassBuffer_DepthMap, pos.xy).r;

	vec3 normal 		= normalize(texture(REN_GB_PrepassBuffer_NormalMap, pos.xy).xyz * 2.0f - 1.0f);
	vec4 clip 			= frag_inverseProjectionViewMatrix * vec4(pos * 2.0f - 1.0f, 1.0f);
	pos 				= clip.xyz / clip.w;

	vec3 lightVec 	= normalize(HEL_Position - pos);
	float cosTheta 	= dot(normal, lightVec);
	float a 		= cosTheta * 0.5f + 0.5f;
	vec3 lightColor = mix(HEL_GroundColor, HEL_SkyColor, a);

	FragColor[0] = vec4(lightColor * HEL_Intensity, 1.0f);
	FragColor[1] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
