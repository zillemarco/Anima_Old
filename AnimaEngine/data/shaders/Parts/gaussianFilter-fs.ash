<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>gaussianFilter-fs</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data name="REN_FilterMap" type="TEXTURE2D" />
				<Data name="REN_TextureSize" type="FLOAT2" />
				<Data name="REN_BlurScale" type="FLOAT2" />
			</Datas>
			<Code>
				<![CDATA[

				#version 150 core

				uniform sampler2D REN_FilterMap;
				uniform vec2 REN_TextureSize;
				uniform vec2 REN_BlurScale;

				out vec4 FragColor;

				void main()
				{
				    vec2 textCoord = gl_FragCoord.xy / REN_TextureSize;
					vec4 color = vec4(0.0f);

					color += texture(REN_FilterMap, textCoord + (vec2(-3.0f) * REN_BlurScale)) * (1.0f / 64.0f);
					color += texture(REN_FilterMap, textCoord + (vec2(-2.0f) * REN_BlurScale)) * (6.0f / 64.0f);
					color += texture(REN_FilterMap, textCoord + (vec2(-1.0f) * REN_BlurScale)) * (15.0f / 64.0f);
					color += texture(REN_FilterMap, textCoord + (vec2(0.0f)  * REN_BlurScale)) * (20.0f / 64.0f);
					color += texture(REN_FilterMap, textCoord + (vec2(1.0f)  * REN_BlurScale)) * (15.0f / 64.0f);
					color += texture(REN_FilterMap, textCoord + (vec2(2.0f)  * REN_BlurScale)) * (6.0f / 64.0f);
					color += texture(REN_FilterMap, textCoord + (vec2(3.0f)  * REN_BlurScale)) * (1.0f / 64.0f);

					FragColor = color;
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>