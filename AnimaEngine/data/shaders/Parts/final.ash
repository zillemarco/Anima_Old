<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>final-fs</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data name="REN_DiffuseMap" type="TEXTURE2D" />
				<Data name="REN_BloomMap" type="TEXTURE2D" />
				<Data name="REN_ScreenSize" type="FLOAT2" />
			</Datas>
			<Code>
				<![CDATA[
				#version 150 core

				uniform sampler2D REN_DiffuseMap;
				uniform sampler2D REN_BloomMap;
				uniform vec2 REN_ScreenSize;

				vec2 CalcTexCoord()
				{
				    return gl_FragCoord.xy / REN_ScreenSize;
				}

				out vec4 FragColor;

				void main()
				{
					vec2 textureCoord = CalcTexCoord();

					vec4 diffuse 	= texture(REN_DiffuseMap, textureCoord);
					vec4 bloom 		= texture(REN_BloomMap, textureCoord);

					FragColor = diffuse + bloom;
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>