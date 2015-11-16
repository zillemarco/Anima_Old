<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>skybox-vs</Name>
	<Type>VS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data propertyName="ProjectionViewMatrix" type="MATRIX4x4" sourceObject="CAMERA"/>
				<Data propertyName="AModelMatrix" type="MATRIX4x4" sourceObject="GEOMETRY"/>
			</Datas>
			<Code>
				<![CDATA[
				#version 150 core

				in vec3 _position;

				uniform mat4 ProjectionViewMatrix;
				uniform mat4 AModelMatrix;

				out vec3 frag_textureCoord;

				void main()
				{
					vec4 position = ProjectionViewMatrix * AModelMatrix * vec4(_position, 1.0);
				    gl_Position = position.xyww;
				    
				    frag_textureCoord = _position;
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>