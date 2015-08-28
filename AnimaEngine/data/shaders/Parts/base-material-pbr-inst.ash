<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>base-material-pbr-fs-inst</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<GroupsData>
				<Group name="MAT" dynamic="false" supportsInstance="true">
					<Datas>
						<Data name="Albedo" type="FLOAT4_ARRAY" size="20"/>
						<Data name="Specular" type="FLOAT4_ARRAY" size="20"/>
						<Data name="Roughness" type="FLOAT_ARRAY" size="20"/>
						<Data name="Metallic" type="FLOAT_ARRAY" size="20"/>
						<Data name="ReflectionIntensity" type="FLOAT_ARRAY" size="20"/>
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
					vec4 Specular[20];
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
					
					// 0.03 default value for dielectic
					vec3 realSpecular = mix(Specular[frag_index].rgb, albedoColor, Metallic[frag_index]);
					
					FragColor[1] = vec4(realAlbedo, Roughness[frag_index]);
					FragColor[2] = vec4(frag_normal * 0.5 + 0.5, ReflectionIntensity[frag_index]);
					FragColor[3] = vec4(realSpecular, Metallic[frag_index]);
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>