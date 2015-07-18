<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader name="base-material-fs" type="FS">
	<Part api="OGL" minVersion="3.3">
		<ShaderCode>
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
		</ShaderCode>
	</Part>
</AnimaShader>