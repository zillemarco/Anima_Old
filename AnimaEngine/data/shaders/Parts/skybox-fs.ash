<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader name="skybox-fs" type="FS">
	<Part api="OGL" minVersion="3.3">
		<ShaderCode>
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
		</ShaderCode>
	</Part>
</AnimaShader>