<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>skybox-fs</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data propertyName="SkyBox" type="TEXTURECUBE" sourceObject="SCENE"/>
			</Datas>
			<Code>
				<![CDATA[
				#version 150 core

				in vec3 frag_textureCoord;
				out vec4 FragColor;

				uniform samplerCube SkyBox;

				void main()
				{
				    vec4 color = texture(SkyBox, frag_textureCoord);
				    FragColor = color * 1.0f;
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>