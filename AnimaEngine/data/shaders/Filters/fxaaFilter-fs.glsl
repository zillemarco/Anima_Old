#version 150 core

uniform sampler2D REN_FilterMap;
uniform vec2 REN_ScreenSize;
uniform vec2 REN_InverseScreenSize;
uniform float REN_FxaaReduceMin;
uniform float REN_FxaaReduceMul;
uniform float REN_FxaaSpanMax;

vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / REN_ScreenSize;
}

out vec4 FragColor;

void main()
{
	vec2 textureCoord = CalcTexCoord();
	vec2 texCoordOffset = REN_InverseScreenSize.xy;
	
	vec3 luma = vec3(0.299, 0.587, 0.114);
	float lumaTL = dot(luma, texture(REN_FilterMap, textureCoord.xy + (vec2(-1.0, -1.0) * texCoordOffset)).xyz);
	float lumaTR = dot(luma, texture(REN_FilterMap, textureCoord.xy + (vec2(1.0, -1.0) * texCoordOffset)).xyz);
	float lumaBL = dot(luma, texture(REN_FilterMap, textureCoord.xy + (vec2(-1.0, 1.0) * texCoordOffset)).xyz);
	float lumaBR = dot(luma, texture(REN_FilterMap, textureCoord.xy + (vec2(1.0, 1.0) * texCoordOffset)).xyz);
	float lumaM  = dot(luma, texture(REN_FilterMap, textureCoord.xy).xyz);

	vec2 dir;
	dir.x = -((lumaTL + lumaTR) - (lumaBL + lumaBR));
	dir.y = ((lumaTL + lumaBL) - (lumaTR + lumaBR));
	
	float dirReduce = max((lumaTL + lumaTR + lumaBL + lumaBR) * (REN_FxaaReduceMul * 0.25), REN_FxaaReduceMin);
	float inverseDirAdjustment = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);
	
	dir = min(vec2(REN_FxaaSpanMax, REN_FxaaSpanMax), 
		max(vec2(-REN_FxaaSpanMax, -REN_FxaaSpanMax), dir * inverseDirAdjustment));
	
	dir.x = dir.x * step(1.0, abs(dir.x));
	dir.y = dir.y * step(1.0, abs(dir.y));
	
	dir = dir * texCoordOffset;

	vec3 result1 = (1.0/2.0) * (
		texture(REN_FilterMap, textureCoord.xy + (dir * vec2(1.0/3.0 - 0.5))).xyz +
		texture(REN_FilterMap, textureCoord.xy + (dir * vec2(2.0/3.0 - 0.5))).xyz);

	vec3 result2 = result1 * (1.0/2.0) + (1.0/4.0) * (
		texture(REN_FilterMap, textureCoord.xy + (dir * vec2(0.0/3.0 - 0.5))).xyz +
		texture(REN_FilterMap, textureCoord.xy + (dir * vec2(3.0/3.0 - 0.5))).xyz);

	float lumaMin = min(lumaM, min(min(lumaTL, lumaTR), min(lumaBL, lumaBR)));
	float lumaMax = max(lumaM, max(max(lumaTL, lumaTR), max(lumaBL, lumaBR)));
	float lumaResult2 = dot(luma, result2);
	
	if(lumaResult2 < lumaMin || lumaResult2 > lumaMax)
		FragColor = vec4(result1, 1.0);
	else
		FragColor = vec4(result2, 1.0);
}