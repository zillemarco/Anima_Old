<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>point-light-vs</Name>
	<Type>VS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data name="CAM_ProjectionViewMatrix" type="MATRIX4x4" />
				<Data name="PTL_LightMeshMatrix" type="MATRIX4x4" />
			</Datas>
			<Code>
				<![CDATA[
				#version 150 core

				in vec3 _position;

				uniform mat4 CAM_ProjectionViewMatrix;
				uniform mat4 PTL_LightMeshMatrix;

				void main()
				{          
				    gl_Position = CAM_ProjectionViewMatrix * PTL_LightMeshMatrix * vec4(_position, 1.0);
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>