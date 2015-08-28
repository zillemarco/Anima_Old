<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>dil-shadow-map-vs-inst</Name>
	<Type>VS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data name="DIL_ProjectionViewMatrix" type="MATRIX4x4" />
			</Datas>
			<GroupsData>
				<Group name="MOD" dynamic="false" supportsInstance="true">
					<Datas>
						<Data name="AModelMatrix" type="MATRIX4x4_ARRAY" size="20"/>
					</Datas>
				</Group>
			</GroupsData>
			<Code>
				<![CDATA[
				#version 150 core

				in vec3 _position;

				uniform mat4 DIL_ProjectionViewMatrix;
				
				layout(std140) uniform MOD
				{
					mat4 AModelMatrix[20];
				};

				void main()
				{
				    gl_Position = DIL_ProjectionViewMatrix * AModelMatrix[gl_InstanceID] * vec4(_position, 1.0);
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>