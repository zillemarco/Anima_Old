<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader name="dil-shadow-map-vs" type="VS">
	<Part api="OGL" minVersion="3.3">
		<ShaderCode>
			<![CDATA[
			#version 150 core

			in vec3 _position;

			uniform mat4 DIL_ProjectionViewMatrix;
			uniform mat4 MOD_AModelMatrix;

			void main()
			{
			    gl_Position = DIL_ProjectionViewMatrix * MOD_AModelMatrix * vec4(_position, 1.0);
			}
			]]>
		</ShaderCode>
	</Part>
</AnimaShader>