#version 330

uniform sampler2D R_FilterMap;
uniform vec2 R_ScreenSize;
uniform vec2 R_InverseScreenSize;
uniform float R_FxaaReduceMin;
uniform float R_FxaaReduceMul;
uniform float R_FxaaSpanMax;

vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / R_ScreenSize;
}

out vec4 FragColor;

void main()
{
	vec2 textureCoord = CalcTexCoord();
	vec2 texCoordOffset = R_InverseScreenSize.xy;
	
	vec3 luma = vec3(0.299, 0.587, 0.114);	
	float lumaTL = dot(luma, texture(R_FilterMap, textureCoord.xy + (vec2(-1.0, -1.0) * texCoordOffset)).xyz);
	float lumaTR = dot(luma, texture(R_FilterMap, textureCoord.xy + (vec2(1.0, -1.0) * texCoordOffset)).xyz);
	float lumaBL = dot(luma, texture(R_FilterMap, textureCoord.xy + (vec2(-1.0, 1.0) * texCoordOffset)).xyz);
	float lumaBR = dot(luma, texture(R_FilterMap, textureCoord.xy + (vec2(1.0, 1.0) * texCoordOffset)).xyz);
	float lumaM  = dot(luma, texture(R_FilterMap, textureCoord.xy).xyz);

	vec2 dir;
	dir.x = -((lumaTL + lumaTR) - (lumaBL + lumaBR));
	dir.y = ((lumaTL + lumaBL) - (lumaTR + lumaBR));
	
	float dirReduce = max((lumaTL + lumaTR + lumaBL + lumaBR) * (R_FxaaReduceMul * 0.25), R_FxaaReduceMin);
	float inverseDirAdjustment = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);
	
	dir = min(vec2(R_FxaaSpanMax, R_FxaaSpanMax), 
		max(vec2(-R_FxaaSpanMax, -R_FxaaSpanMax), dir * inverseDirAdjustment));
	
	dir.x = dir.x * step(1.0, abs(dir.x));
	dir.y = dir.y * step(1.0, abs(dir.y));
	
	dir = dir * texCoordOffset;

	vec3 result1 = (1.0/2.0) * (
		texture(R_FilterMap, textureCoord.xy + (dir * vec2(1.0/3.0 - 0.5))).xyz +
		texture(R_FilterMap, textureCoord.xy + (dir * vec2(2.0/3.0 - 0.5))).xyz);

	vec3 result2 = result1 * (1.0/2.0) + (1.0/4.0) * (
		texture(R_FilterMap, textureCoord.xy + (dir * vec2(0.0/3.0 - 0.5))).xyz +
		texture(R_FilterMap, textureCoord.xy + (dir * vec2(3.0/3.0 - 0.5))).xyz);

	float lumaMin = min(lumaM, min(min(lumaTL, lumaTR), min(lumaBL, lumaBR)));
	float lumaMax = max(lumaM, max(max(lumaTL, lumaTR), max(lumaBL, lumaBR)));
	float lumaResult2 = dot(luma, result2);
	
	if(lumaResult2 < lumaMin || lumaResult2 > lumaMax)
		FragColor = vec4(result1, 1.0);
	else
		FragColor = vec4(result2, 1.0);
}