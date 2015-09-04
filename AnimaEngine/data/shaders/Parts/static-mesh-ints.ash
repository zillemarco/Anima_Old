<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>static-mesh-vs-inst</Name>
	<Type>VS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data name="CAM_ProjectionViewMatrix" type="MATRIX4x4" />
			</Datas>
			<GroupsData>
				<Group name="MOD" dynamic="false" supportsInstance="true">
					<Datas>
						<Data name="AModelMatrix" type="MATRIX4x4_ARRAY" size="20"/>
						<Data name="ANormalMatrix" type="MATRIX4x4_ARRAY" size="20"/>
					</Datas>
				</Group>
			</GroupsData>
			<Code>
				<![CDATA[
				#version 150 core

				in vec3 _position;
				in vec3 _normal;
				in vec2 _textureCoord;
				in vec3 _tangent;
				in vec3 _bitangent;

				uniform mat4 CAM_ProjectionViewMatrix;

				layout(std140) uniform MOD
				{
					mat4 AModelMatrix[20];
					mat4 ANormalMatrix[20];
				};

				out vec2 frag_textureCoord;
				out vec3 frag_normal;
				out mat3 frag_TBNMatrix;
				flat out int frag_index;

				void main()
				{
					frag_index = gl_InstanceID;

				    gl_Position = CAM_ProjectionViewMatrix *  AModelMatrix[frag_index] * vec4(_position, 1.0);
				    
					frag_normal = normalize((ANormalMatrix[frag_index] * vec4(_normal, 0.0)).xyz);
				    vec3 tangent = normalize((ANormalMatrix[frag_index] * vec4(_tangent, 0.0)).xyz);
				    vec3 bitangent = normalize((ANormalMatrix[frag_index] * vec4(_bitangent, 0.0)).xyz);

				    frag_textureCoord = _textureCoord;
					frag_TBNMatrix = mat3(tangent, bitangent, frag_normal);
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>