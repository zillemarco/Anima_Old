<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>base-material-fs</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data name="MAT_DiffuseColor" type="FLOAT4" />
				<Data name="MAT_SpecularColor" type="FLOAT3" />
				<Data name="MAT_Shininess" type="FLOAT" />
			</Datas>
			<Code>
				<![CDATA[
				#version 410 core

				in vec3 frag_normal;

				uniform vec4 MAT_DiffuseColor;
				uniform vec3 MAT_SpecularColor;
				uniform float MAT_Shininess;

				out vec4 FragColor[4];

				void main()
				{
					vec4 diffuseColor = MAT_DiffuseColor;
					vec3 specularColor = MAT_SpecularColor;

					FragColor[1] = diffuseColor;
					FragColor[2] = vec4(frag_normal * 0.5 + 0.5, 1.0);
					FragColor[3] = vec4(specularColor, 1.0 / MAT_Shininess);
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>