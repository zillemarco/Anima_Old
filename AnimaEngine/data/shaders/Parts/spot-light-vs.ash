<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>spot-light-vs</Name>
	<Type>VS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data propertyName="InverseProjectionViewMatrix" type="MATRIX4x4" sourceObject="CAMERA"/>
				<Data propertyName="ProjectionViewMatrix" type="MATRIX4x4" sourceObject="CAMERA"/>
				<Data propertyName="AModelMatrix" type="MATRIX4x4" sourceObject="GEOMETRY"/>
			</Datas>
			<Code>
				<![CDATA[
				#version 150 core

				in vec3 _position;

				uniform mat4 InverseProjectionViewMatrix;
				uniform mat4 ProjectionViewMatrix;
				uniform mat4 AModelMatrix;

				out mat4 frag_inverseProjectionViewMatrix;

				void main()
				{          
				    gl_Position = ProjectionViewMatrix * AModelMatrix * vec4(_position, 1.0);
				    frag_inverseProjectionViewMatrix = InverseProjectionViewMatrix;
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>