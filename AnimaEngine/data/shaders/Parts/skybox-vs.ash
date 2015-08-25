<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>skybox-vs</Name>
	<Type>VS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data name="CAM_ProjectionViewMatrix" type="MATRIX4x4" />
				<Data name="MOD_AModelMatrix" type="MATRIX4x4" />
			</Datas>
			<Code>
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
			</Code>
		</Version>
	</Versions>
</AnimaShader>