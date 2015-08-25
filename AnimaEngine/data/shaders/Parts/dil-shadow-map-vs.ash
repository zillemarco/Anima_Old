<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>dil-shadow-map-vs</Name>
	<Type>VS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data name="DIL_ProjectionViewMatrix" type="MATRIX4x4" />
				<Data name="MOD_AModelMatrix" type="MATRIX4x4" />
			</Datas>
			<Code>
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
			</Code>
		</Version>
	</Versions>
</AnimaShader>