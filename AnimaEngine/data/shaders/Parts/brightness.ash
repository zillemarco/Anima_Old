<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>brightnessFilter-fs</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data name="REN_FilterMap" type="TEXTURE2D" />
				<Data name="REN_DestinationTextureSize" type="FLOAT2" />
				<Data name="REN_BrightnessThreshold" type="FLOAT" />
			</Datas>
			<Code>
				<![CDATA[
				#version 150 core

				uniform sampler2D REN_FilterMap;
				uniform vec2 REN_DestinationTextureSize;
				uniform float REN_BrightnessThreshold;

				out vec4 FragColor;

				void main()
				{
					vec3 luma = vec3(0.299, 0.587, 0.114);
					vec2 textCoord = gl_FragCoord.xy / REN_DestinationTextureSize;

					vec3 color = texture(REN_FilterMap, textCoord).xyz;

					float value = dot(color, luma);

					if(value > REN_BrightnessThreshold)
						FragColor = vec4(color, 1.0);
					else
						discard;
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>