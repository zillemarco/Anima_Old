#version 330

uniform sampler2D _renderingFilterMap;
uniform vec2 _renderingScreenSize;
uniform vec2 _renderingInverseScreenSize;
uniform float _renderingFxaaReduceMin;
uniform float _renderingFxaaReduceMul;
uniform float _renderingFxaaSpanMax;

vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / _renderingScreenSize;
}

out vec4 FragColor;

void main()
{
	vec2 textureCoord = CalcTexCoord();
	vec2 texCoordOffset = _renderingInverseScreenSize.xy;
	
	vec3 luma = vec3(0.299, 0.587, 0.114);	
	float lumaTL = dot(luma, texture(_renderingFilterMap, textureCoord.xy + (vec2(-1.0, -1.0) * texCoordOffset)).xyz);
	float lumaTR = dot(luma, texture(_renderingFilterMap, textureCoord.xy + (vec2(1.0, -1.0) * texCoordOffset)).xyz);
	float lumaBL = dot(luma, texture(_renderingFilterMap, textureCoord.xy + (vec2(-1.0, 1.0) * texCoordOffset)).xyz);
	float lumaBR = dot(luma, texture(_renderingFilterMap, textureCoord.xy + (vec2(1.0, 1.0) * texCoordOffset)).xyz);
	float lumaM  = dot(luma, texture(_renderingFilterMap, textureCoord.xy).xyz);

	vec2 dir;
	dir.x = -((lumaTL + lumaTR) - (lumaBL + lumaBR));
	dir.y = ((lumaTL + lumaBL) - (lumaTR + lumaBR));
	
	float dirReduce = max((lumaTL + lumaTR + lumaBL + lumaBR) * (_renderingFxaaReduceMul * 0.25), _renderingFxaaReduceMin);
	float inverseDirAdjustment = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);
	
	dir = min(vec2(_renderingFxaaSpanMax, _renderingFxaaSpanMax), 
		max(vec2(-_renderingFxaaSpanMax, -_renderingFxaaSpanMax), dir * inverseDirAdjustment));
	
	dir.x = dir.x * step(1.0, abs(dir.x));
	dir.y = dir.y * step(1.0, abs(dir.y));
	
	dir = dir * texCoordOffset;

	vec3 result1 = (1.0/2.0) * (
		texture(_renderingFilterMap, textureCoord.xy + (dir * vec2(1.0/3.0 - 0.5))).xyz +
		texture(_renderingFilterMap, textureCoord.xy + (dir * vec2(2.0/3.0 - 0.5))).xyz);

	vec3 result2 = result1 * (1.0/2.0) + (1.0/4.0) * (
		texture(_renderingFilterMap, textureCoord.xy + (dir * vec2(0.0/3.0 - 0.5))).xyz +
		texture(_renderingFilterMap, textureCoord.xy + (dir * vec2(3.0/3.0 - 0.5))).xyz);

	float lumaMin = min(lumaM, min(min(lumaTL, lumaTR), min(lumaBL, lumaBR)));
	float lumaMax = max(lumaM, max(max(lumaTL, lumaTR), max(lumaBL, lumaBR)));
	float lumaResult2 = dot(luma, result2);
	
	if(lumaResult2 < lumaMin || lumaResult2 > lumaMax)
		FragColor = vec4(result1, 1.0);
	else
		FragColor = vec4(result2, 1.0);
}