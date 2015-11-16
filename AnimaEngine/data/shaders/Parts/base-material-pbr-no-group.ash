<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>base-material-pbr-fs-no-group</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data propertyName="Albedo" type="FLOAT4" sourceObject="MATERIAL"/>
				<Data propertyName="Specular" type="FLOAT" sourceObject="MATERIAL"/>
				<Data propertyName="Roughness" type="FLOAT" sourceObject="MATERIAL"/>
				<Data propertyName="Metallic" type="FLOAT" sourceObject="MATERIAL"/>
				<Data propertyName="ReflectionIntensity" type="FLOAT" sourceObject="MATERIAL"/>
			</Datas>
			<Code>
				<![CDATA[
				#version 410 core

				in vec3 frag_normal;

				uniform	vec4 Albedo;
				uniform	float Specular;
				uniform	float Roughness;
				uniform	float Metallic;
				uniform	float ReflectionIntensity;

				out vec4 FragColor[4];

				void main()
				{
					vec3 albedoColor = Albedo.rgb;
					
					// gamma correction
					albedoColor = pow(albedoColor.rgb, vec3(2.2f));
					
					// Lerp with metallic
					vec3 realAlbedo = albedoColor - (albedoColor * Metallic);
					
					float spec = 0.02 + MAT_Specular * 0.03;
					vec3 realSpecular = mix(vec3(spec), albedoColor, Metallic);
					
					FragColor[1] = vec4(realAlbedo, Roughness);
					FragColor[2] = vec4(frag_normal * 0.5 + 0.5, ReflectionIntensity);
					FragColor[3] = vec4(realSpecular, Metallic);
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>