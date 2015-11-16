<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>gaussianFilter-fs</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data propertyName="FilterMap" type="TEXTURE2D" sourceObject="RENDERER"/>
				<Data propertyName="TextureSize" type="FLOAT2" sourceObject="RENDERER"/>
				<Data propertyName="BlurScale" type="FLOAT2" sourceObject="RENDERER"/>
			</Datas>
			<Code>
				<![CDATA[

				#version 150 core

				uniform sampler2D FilterMap;
				uniform vec2 TextureSize;
				uniform vec2 BlurScale;

				out vec4 FragColor;

				void main()
				{
				    vec2 textCoord = gl_FragCoord.xy / TextureSize;
					vec4 color = vec4(0.0f);

					color += texture(FilterMap, textCoord + (vec2(-3.0f) * BlurScale)) * (1.0f / 64.0f);
					color += texture(FilterMap, textCoord + (vec2(-2.0f) * BlurScale)) * (6.0f / 64.0f);
					color += texture(FilterMap, textCoord + (vec2(-1.0f) * BlurScale)) * (15.0f / 64.0f);
					color += texture(FilterMap, textCoord + (vec2(0.0f)  * BlurScale)) * (20.0f / 64.0f);
					color += texture(FilterMap, textCoord + (vec2(1.0f)  * BlurScale)) * (15.0f / 64.0f);
					color += texture(FilterMap, textCoord + (vec2(2.0f)  * BlurScale)) * (6.0f / 64.0f);
					color += texture(FilterMap, textCoord + (vec2(3.0f)  * BlurScale)) * (1.0f / 64.0f);

					FragColor = color;
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>