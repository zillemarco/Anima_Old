<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>directional-light-vs</Name>
	<Type>VS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data name="DIL_LightMeshMatrix" type="MATRIX4x4" />
			</Datas>
			<Code>
				<![CDATA[
				#version 150 core

				in vec3 _position;

				uniform mat4 DIL_LightMeshMatrix;

				void main()
				{          
				    gl_Position = DIL_LightMeshMatrix * vec4(_position, 1.0);
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>