<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>nullFilter-fs</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data propertyName="FilterMap" type="TEXTURE" sourceObject="RENDERER"/>
				<Data propertyName="ScreenSize" type="FLOAT2" sourceObject="RENDERER"/>
			</Datas>
			<Code>
				<![CDATA[
				#version 150 core

				uniform sampler2D FilterMap;
				uniform vec2 ScreenSize;

				vec2 CalcTexCoord()
				{
				    return gl_FragCoord.xy / ScreenSize;
				}

				out vec4 FragColor;

				void main()
				{
				    vec2 TexCoord = CalcTexCoord();
					vec3 Color = texture(FilterMap, TexCoord).xyz;

					FragColor = vec4(Color, 1.0);
				}			
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>