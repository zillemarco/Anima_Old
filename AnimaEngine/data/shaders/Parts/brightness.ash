<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>brightnessFilter-fs</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data propertyName="FilterMap" type="TEXTURE2D" sourceObject="RENDERER"/>
				<Data propertyName="DestinationTextureSize" type="FLOAT2" sourceObject="RENDERER"/>
				<Data propertyName="BrightnessThreshold" type="FLOAT" sourceObject="RENDERER"/>
			</Datas>
			<Code>
				<![CDATA[
				#version 150 core

				uniform sampler2D FilterMap;
				uniform vec2 DestinationTextureSize;
				uniform float BrightnessThreshold;

				out vec4 FragColor;

				void main()
				{
					vec3 luma = vec3(0.299, 0.587, 0.114);
					vec2 textCoord = gl_FragCoord.xy / DestinationTextureSize;

					vec3 color = texture(FilterMap, textCoord).xyz;

					float value = dot(color, luma);

					if(value > BrightnessThreshold)
						FragColor = vec4(color, 1.0);
					else
						discard;
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>