#version 150 core

uniform sampler2D REN_GB_PrepassBuffer_DepthMap;
uniform sampler2D REN_Diffuse2Map;
uniform sampler2D REN_GB_PrimitivesBuffer_DepthMap;
uniform sampler2D REN_GB_PrimitivesBuffer_ColorMap;
uniform vec2 REN_ScreenSize;
uniform vec2 REN_InverseScreenSize;

vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / REN_ScreenSize;
}

out vec4 FragColor;

void main()
{
	vec2 textureCoord = CalcTexCoord();

	float depthPrimitive = texture(REN_GB_PrimitivesBuffer_DepthMap, textureCoord).r;
	float depthDraw = texture(REN_GB_PrimitivesBuffer_DepthMap, textureCoord).r;

	if(depthPrimitive < depthDraw)
		FragColor = texture(REN_GB_PrimitivesBuffer_ColorMap, textureCoord);
	else
		FragColor = texture(REN_Diffuse2Map, textureCoord);
}