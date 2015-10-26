<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>physicsDebug-vs</Name>
	<Type>VS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data name="CAM_ProjectionViewMatrix" type="MATRIX4x4" />
			</Datas>
			<Code>
				<![CDATA[
				#version 150 core

				in vec3 _position;
				in vec3 _color;

				uniform mat4 CAM_ProjectionViewMatrix;

				out vec3 frag_color;

				void main()
				{          
				    gl_Position = CAM_ProjectionViewMatrix * vec4(_position, 1.0);
				    frag_color = _color;
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>