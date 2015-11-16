<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>final-fs</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data propertyName="DiffuseMap" type="TEXTURE2D" sourceObject="RENDERER"/>
				<Data propertyName="BloomMap" type="TEXTURE2D" sourceObject="RENDERER"/>
				<Data propertyName="ScreenSize" type="FLOAT2" sourceObject="RENDERER"/>
			</Datas>
			<Code>
				<![CDATA[
				#version 150 core

				uniform sampler2D DiffuseMap;
				uniform sampler2D BloomMap;
				uniform vec2 ScreenSize;

				vec2 CalcTexCoord()
				{
				    return gl_FragCoord.xy / ScreenSize;
				}

				out vec4 FragColor;

				void main()
				{
					vec2 textureCoord = CalcTexCoord();

					vec4 diffuse 	= texture(DiffuseMap, textureCoord);
					vec4 bloom 		= texture(BloomMap, textureCoord);

					FragColor = diffuse + bloom;
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>