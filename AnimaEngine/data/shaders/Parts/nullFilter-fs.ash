<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>nullFilter-fs</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data name="REN_FilterMap" type="TEXTURE" />
				<Data name="REN_ScreenSize" type="FLOAT2" />
			</Datas>
			<Code>
				<![CDATA[
				#version 150 core

				uniform sampler2D REN_FilterMap;
				uniform vec2 REN_ScreenSize;

				vec2 CalcTexCoord()
				{
				    return gl_FragCoord.xy / REN_ScreenSize;
				}

				out vec4 FragColor;

				void main()
				{
				    vec2 TexCoord = CalcTexCoord();
					vec3 Color = texture(REN_FilterMap, TexCoord).xyz;

					FragColor = vec4(Color, 1.0);
				}			
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>