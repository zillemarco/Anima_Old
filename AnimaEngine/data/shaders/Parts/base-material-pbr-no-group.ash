<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>base-material-pbr-fs-no-group</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data name="MAT_Albedo" type="FLOAT4" />
				<Data name="MAT_Specular" type="FLOAT" />
				<Data name="MAT_Roughness" type="FLOAT" />
				<Data name="MAT_Metallic" type="FLOAT" />
				<Data name="MAT_ReflectionIntensity" type="FLOAT" />
			</Datas>
			<Code>
				<![CDATA[
				#version 410 core

				in vec3 frag_normal;

				uniform	vec4 MAT_Albedo;
				uniform	float MAT_Specular;
				uniform	float MAT_Roughness;
				uniform	float MAT_Metallic;
				uniform	float MAT_ReflectionIntensity;

				out vec4 FragColor[4];

				void main()
				{
					vec3 albedoColor = MAT_Albedo.rgb;
					
					// gamma correction
					albedoColor = pow(albedoColor.rgb, vec3(2.2f));
					
					// Lerp with metallic
					vec3 realAlbedo = albedoColor - (albedoColor * MAT_Metallic);
					
					float spec = 0.02 + MAT_Specular * 0.03;
					vec3 realSpecular = mix(vec3(spec), albedoColor, MAT_Metallic);
					
					FragColor[1] = vec4(realAlbedo, MAT_Roughness);
					FragColor[2] = vec4(frag_normal * 0.5 + 0.5, MAT_ReflectionIntensity);
					FragColor[3] = vec4(realSpecular, MAT_Metallic);
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>