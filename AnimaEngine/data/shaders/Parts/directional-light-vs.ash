<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>directional-light-vs</Name>
	<Type>VS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data propertyName="LightGeometryMatrix" type="MATRIX4x4" sourceObject="LIGHT"/>
			</Datas>
			<Code>
				<![CDATA[
				#version 150 core

				in vec3 _position;

				uniform mat4 LightGeometryMatrix;

				void main()
				{          
				    gl_Position = LightGeometryMatrix * vec4(_position, 1.0);
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>