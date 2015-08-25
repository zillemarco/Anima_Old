<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>directional-light-vs</Name>
	<Type>VS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data name="MOD_AModelMatrix" type="MATRIX4x4" />
			</Datas>
			<Code>
				<![CDATA[
				#version 150 core

				in vec3 _position;

				uniform mat4 MOD_AModelMatrix;

				void main()
				{          
				    gl_Position = MOD_AModelMatrix * vec4(_position, 1.0);
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>