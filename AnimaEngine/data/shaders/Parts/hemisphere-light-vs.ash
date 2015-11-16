<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>hemisphere-light-vs</Name>
	<Type>VS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data propertyName="AModelMatrix" type="MATRIX4x4" sourceObject="GEOMETRY"/>
			</Datas>
			<Code>
				<![CDATA[
				#version 150 core

				in vec3 _position;

				uniform mat4 AModelMatrix;

				void main()
				{          
				    gl_Position = AModelMatrix * vec4(_position, 1.0);
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>