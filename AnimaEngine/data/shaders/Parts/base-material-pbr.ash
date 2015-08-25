<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>base-material-pbr-fs</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<GroupsData>
				<Group name="MAT" dynamic="false">
					<Datas>
						<Data name="Albedo" type="FLOAT4" />
						<Data name="Specular" type="FLOAT4" />
						<Data name="Roughness" type="FLOAT" />
						<Data name="Metallic" type="FLOAT" />
						<Data name="ReflectionIntensity" type="FLOAT" />
					</Datas>
				</Group>
			</GroupsData>
			<Code>
				<![CDATA[
				#version 410 core

				in vec3 frag_normal;

				layout(std140) uniform MAT
				{
					vec4 MAT_Albedo;
					vec4 MAT_Specular;
					float MAT_Roughness;
					float MAT_Metallic;
					float MAT_ReflectionIntensity;
				};

				out vec4 FragColor[4];

				void main()
				{
					vec3 albedoColor = MAT_Albedo.rgb;
					
					// gamma correction
					albedoColor = pow(albedoColor.rgb, vec3(2.2f));
					
					// Lerp with metallic
					vec3 realAlbedo = albedoColor - (albedoColor * MAT_Metallic);
					
					// 0.03 default value for dielectic
					vec3 realSpecular = mix(MAT_Specular.rgb, albedoColor, MAT_Metallic);
					
					FragColor[1] = vec4(realAlbedo, MAT_Roughness);
					FragColor[2] = vec4(frag_normal * 0.5 + 0.5, MAT_ReflectionIntensity);
					FragColor[3] = vec4(realSpecular, MAT_Metallic);
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>