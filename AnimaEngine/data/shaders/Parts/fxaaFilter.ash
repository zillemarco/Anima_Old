<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>fxaaFilter-fs</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data propertyName="FilterMap" type="TEXTURE2D" sourceObject="RENDERER"/>
				<Data propertyName="ScreenSize" type="FLOAT2" sourceObject="RENDERER"/>
				<Data propertyName="InverseScreenSize" type="FLOAT2" sourceObject="RENDERER"/>
				<Data propertyName="FxaaReduceMin" type="FLOAT" sourceObject="RENDERER"/>
				<Data propertyName="FxaaReduceMul" type="FLOAT" sourceObject="RENDERER"/>
				<Data propertyName="FxaaSpanMax" type="FLOAT" sourceObject="RENDERER"/>
			</Datas>
			<Code>
				<![CDATA[

					#version 150 core

					uniform sampler2D FilterMap;
					uniform vec2 ScreenSize;
					uniform vec2 InverseScreenSize;
					uniform float FxaaReduceMin;
					uniform float FxaaReduceMul;
					uniform float FxaaSpanMax;

					out vec4 FragColor;

					void main()
					{
						vec2 textureCoord = gl_FragCoord.xy / ScreenSize;
						vec2 texCoordOffset = InverseScreenSize.xy;
						
						vec3 luma = vec3(0.299, 0.587, 0.114);
						float lumaTL = dot(luma, texture(FilterMap, textureCoord.xy + (vec2(-1.0, -1.0) * texCoordOffset)).xyz);
						float lumaTR = dot(luma, texture(FilterMap, textureCoord.xy + (vec2(1.0, -1.0) * texCoordOffset)).xyz);
						float lumaBL = dot(luma, texture(FilterMap, textureCoord.xy + (vec2(-1.0, 1.0) * texCoordOffset)).xyz);
						float lumaBR = dot(luma, texture(FilterMap, textureCoord.xy + (vec2(1.0, 1.0) * texCoordOffset)).xyz);
						float lumaM  = dot(luma, texture(FilterMap, textureCoord.xy).xyz);

						vec2 dir;
						dir.x = -((lumaTL + lumaTR) - (lumaBL + lumaBR));
						dir.y = ((lumaTL + lumaBL) - (lumaTR + lumaBR));
						
						float dirReduce = max((lumaTL + lumaTR + lumaBL + lumaBR) * (FxaaReduceMul * 0.25), FxaaReduceMin);
						float inverseDirAdjustment = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);
						
						dir = min(vec2(FxaaSpanMax, FxaaSpanMax), 
							max(vec2(-FxaaSpanMax, -FxaaSpanMax), dir * inverseDirAdjustment));
						
						dir.x = dir.x * step(1.0, abs(dir.x));
						dir.y = dir.y * step(1.0, abs(dir.y));
						
						dir = dir * texCoordOffset;

						vec3 result1 = (1.0/2.0) * (
							texture(FilterMap, textureCoord.xy + (dir * vec2(1.0/3.0 - 0.5))).xyz +
							texture(FilterMap, textureCoord.xy + (dir * vec2(2.0/3.0 - 0.5))).xyz);

						vec3 result2 = result1 * (1.0/2.0) + (1.0/4.0) * (
							texture(FilterMap, textureCoord.xy + (dir * vec2(0.0/3.0 - 0.5))).xyz +
							texture(FilterMap, textureCoord.xy + (dir * vec2(3.0/3.0 - 0.5))).xyz);

						float lumaMin = min(lumaM, min(min(lumaTL, lumaTR), min(lumaBL, lumaBR)));
						float lumaMax = max(lumaM, max(max(lumaTL, lumaTR), max(lumaBL, lumaBR)));
						float lumaResult2 = dot(luma, result2);
						
						if(lumaResult2 < lumaMin || lumaResult2 > lumaMax)
							FragColor = vec4(result1, 1.0);
						else
							FragColor = vec4(result2, 1.0);
					}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>