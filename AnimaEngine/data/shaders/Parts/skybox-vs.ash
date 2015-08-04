<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader name="skybox-vs" type="VS">
	<Part api="OGL" minVersion="3.3">
		<ShaderCode>
			<![CDATA[
			#version 150 core

			in vec3 _position;

			uniform mat4 CAM_ProjectionViewMatrix;
			uniform mat4 MOD_AModelMatrix;

			out vec3 frag_textureCoord;

			void main()
			{
				vec4 position = CAM_ProjectionViewMatrix *  MOD_AModelMatrix * vec4(_position, 1.0);
			    gl_Position = position.xyww;
			    
			    frag_textureCoord = _position;
			}
			]]>
		</ShaderCode>
	</Part>
</AnimaShader>