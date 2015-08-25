<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>static-mesh-vs</Name>
	<Type>VS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data name="CAM_ProjectionViewMatrix" type="MATRIX4x4" />
			</Datas>
			<GroupsData>
				<Group name="MOD" dynamic="true">
					<Datas>
						<Data name="AModelMatrix" type="MATRIX4x4" />
						<Data name="ANormalMatrix" type="MATRIX4x4" />
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
					mat4 MOD_AModelMatrix;
					mat4 MOD_ANormalMatrix;
				};

				out vec2 frag_textureCoord;
				out vec3 frag_normal;
				out vec3 frag_worldPosition;
				out mat3 frag_TBNMatrix;

				void main()
				{
				    gl_Position = CAM_ProjectionViewMatrix *  MOD_AModelMatrix * vec4(_position, 1.0);
				    
					frag_normal = normalize((MOD_ANormalMatrix * vec4(_normal, 0.0)).xyz);
				    vec3 tangent = normalize((MOD_ANormalMatrix * vec4(_tangent, 0.0)).xyz);
				    vec3 bitangent = normalize((MOD_ANormalMatrix * vec4(_bitangent, 0.0)).xyz);
					frag_worldPosition = (MOD_AModelMatrix * vec4(_position, 1.0)).xyz;
				    frag_textureCoord = _textureCoord;
					frag_TBNMatrix = mat3(tangent, bitangent, frag_normal);
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>