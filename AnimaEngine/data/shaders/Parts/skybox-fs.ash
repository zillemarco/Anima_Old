<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>skybox-fs</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data name="REN_SkyBox" type="TEXTURECUBE" />
			</Datas>
			<Code>
				<![CDATA[
				#version 150 core

				in vec3 frag_textureCoord;
				out vec4 FragColor;

				uniform samplerCube REN_SkyBox;

				void main()
				{
				    vec4 color = texture(REN_SkyBox, frag_textureCoord);
				    FragColor = color * 1.0f;
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>