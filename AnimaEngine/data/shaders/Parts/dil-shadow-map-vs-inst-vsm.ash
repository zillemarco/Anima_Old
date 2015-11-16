<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>dil-shadow-map-vs-inst-vsm</Name>
	<Type>VS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data propertyName="ProjectionViewMatrix" type="MATRIX4x4" sourceObject="LIGHT"/>
			</Datas>
			<GroupsData>
				<Group groupName="MOD" dynamic="false" supportsInstance="true" sourceObject="GEOMETRY">
					<Datas>
						<Data propertyName="AModelMatrix" type="MATRIX4x4_ARRAY" size="20"/>
					</Datas>
				</Group>
			</GroupsData>
			<Code>
				<![CDATA[
				#version 150 core

				in vec3 _position;

				uniform mat4 ProjectionViewMatrix;
				
				layout(std140) uniform MOD
				{
					mat4 AModelMatrix[20];
				};

				void main()
				{
				    gl_Position = ProjectionViewMatrix * AModelMatrix[gl_InstanceID] * vec4(_position, 1.0);
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>