#version 150 core

out vec4 FragColor;

uniform sampler2D REN_GB_FilterBuffer_DepthMap;
uniform sampler2D REN_GB_FilterBuffer_NormalMap;
uniform vec2 REN_InverseScreenSize;
uniform mat4 CAM_ProjectionViewInverseMatrix;

uniform vec2 REN_SSAOFilterRadius;
uniform float REN_SSAODistanceThreshold;

const int sample_count = 16;
const vec2 poisson16[] = vec2[](    // These are the Poisson Disk Samples
                                vec2( -0.94201624,  -0.39906216 ),
                                vec2(  0.94558609,  -0.76890725 ),
                                vec2( -0.094184101, -0.92938870 ),
                                vec2(  0.34495938,   0.29387760 ),
                                vec2( -0.91588581,   0.45771432 ),
                                vec2( -0.81544232,  -0.87912464 ),
                                vec2( -0.38277543,   0.27676845 ),
                                vec2(  0.97484398,   0.75648379 ),
                                vec2(  0.44323325,  -0.97511554 ),
                                vec2(  0.53742981,  -0.47373420 ),
                                vec2( -0.26496911,  -0.41893023 ),
                                vec2(  0.79197514,   0.19090188 ),
                                vec2( -0.24188840,   0.99706507 ),
                                vec2( -0.81409955,   0.91437590 ),
                                vec2(  0.19984126,   0.78641367 ),
                                vec2(  0.14383161,  -0.14100790 )
                               );

vec3 calcPosition(vec2 textureCoord, float depth)
{
	vec3 genPos = vec3(textureCoord, depth);
	vec4 clip = CAM_ProjectionViewInverseMatrix * vec4(genPos * 2.0f - 1.0f, 1.0f);
	return clip.xyz / clip.w;
}

vec3 calcNormal(vec2 textureCoord)
{
	return normalize(texture(REN_GB_FilterBuffer_NormalMap, vec2((textureCoord.x * REN_InverseScreenSize.x), (textureCoord.y * REN_InverseScreenSize.y))).xyz * 2.0f - 1.0f);
}

float calcDepth(vec2 textureCoord)
{
	return texture(REN_GB_FilterBuffer_DepthMap, textureCoord).r;
}

void main()
{
	float depth = calcDepth(gl_FragCoord.xy * REN_InverseScreenSize);
	vec3 normal = calcNormal(gl_FragCoord.xy * REN_InverseScreenSize);
	vec3 pos 	= calcPosition(gl_FragCoord.xy * REN_InverseScreenSize, depth);

	float ao = 0.0f;

	for(int i = 0; i < sample_count; i++)
	{
		vec2 sampleTexCoord = (gl_FragCoord.xy + (poisson16[i] * REN_SSAOFilterRadius)) * REN_InverseScreenSize;
		float sampleDepth = calcDepth(sampleTexCoord);
		vec3 samplePos = calcPosition(sampleTexCoord, sampleDepth);
		vec3 sampleDir = normalize(samplePos - pos);

		float NdotS = max(dot(normal, sampleDir), 0.0f);
		float VPDistSP = distance(pos, samplePos);

		float a = 1.0f - smoothstep(REN_SSAODistanceThreshold, REN_SSAODistanceThreshold * 2.0f, VPDistSP);
		float b = NdotS;

		ao += (a * b);
	}

	float ambientOcclusion = 1.0f - (ao / sample_count);
	FragColor = vec4(ambientOcclusion, 0.0f, 0.0f, 1.0f);
}
