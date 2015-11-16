<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>base-material-fs</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data propertyName="DiffuseColor" type="FLOAT4" sourceObject="MATERIAL"/>
				<Data propertyName="SpecularColor" type="FLOAT3" sourceObject="MATERIAL"/>
				<Data propertyName="Shininess" type="FLOAT" sourceObject="MATERIAL"/>
			</Datas>
			<Code>
				<![CDATA[
				#version 410 core

				in vec3 frag_normal;

				uniform vec4 DiffuseColor;
				uniform vec3 SpecularColor;
				uniform float Shininess;

				out vec4 FragColor[4];

				void main()
				{
					vec4 diffuseColor = DiffuseColor;
					vec3 specularColor = SpecularColor;

					FragColor[1] = diffuseColor;
					FragColor[2] = vec4(frag_normal * 0.5 + 0.5, 1.0);
					FragColor[3] = vec4(specularColor, 1.0 / Shininess);
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>