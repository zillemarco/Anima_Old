<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader name="directional-light-vs" type="VS">
	<Part api="OGL" minVersion="3.3">
		<ShaderCode>
			<![CDATA[
			#version 150 core

			in vec3 _position;

			uniform mat4 MOD_AModelMatrix;

			void main()
			{          
			    gl_Position = MOD_AModelMatrix * vec4(_position, 1.0);
			}
			]]>
		</ShaderCode>
	</Part>
</AnimaShader>