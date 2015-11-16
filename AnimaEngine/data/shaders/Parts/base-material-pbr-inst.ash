<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>base-material-pbr-fs-inst</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<GroupsData>
				<Group groupName="MAT" dynamic="false" supportsInstance="true" sourceObject="MATERIAL">
					<Datas>
						<Data propertyName="Albedo" type="FLOAT4_ARRAY" size="20"/>
						<Data propertyName="Specular" type="FLOAT_ARRAY" size="20"/>
						<Data propertyName="Roughness" type="FLOAT_ARRAY" size="20"/>
						<Data propertyName="Metallic" type="FLOAT_ARRAY" size="20"/>
						<Data propertyName="ReflectionIntensity" type="FLOAT_ARRAY" size="20"/>
					</Datas>
				</Group>
			</GroupsData>
			<Code>
				<![CDATA[
				#version 410 core

				in vec3 frag_normal;
				flat in int frag_index;

				layout(std140) uniform MAT
				{
					vec4 Albedo[20];
					float Specular[20];
					float Roughness[20];
					float Metallic[20];
					float ReflectionIntensity[20];
				};

				out vec4 FragColor[4];

				void main()
				{
					vec3 albedoColor = Albedo[frag_index].rgb;
					
					// gamma correction
					albedoColor = pow(albedoColor.rgb, vec3(2.2f));
					
					// Lerp with metallic
					vec3 realAlbedo = albedoColor - (albedoColor * Metallic[frag_index]);
					
					float spec = 0.02 + Specular[frag_index] * 0.03;
					vec3 realSpecular = mix(vec3(spec), albedoColor, Metallic[frag_index]);
					
					FragColor[1] = vec4(realAlbedo, Roughness[frag_index]);
					FragColor[2] = vec4(frag_normal * 0.5 + 0.5, ReflectionIntensity[frag_index]);
					FragColor[3] = vec4(realSpecular, Metallic[frag_index]);
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>