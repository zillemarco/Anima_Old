<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>base-material-pbr-fs-inst-texture</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data propertyName="AlbedoMap" type="TEXTURE2D" sourceObject="MATERIAL"/>
				<Data propertyName="NormalMap" type="TEXTURE2D" sourceObject="MATERIAL"/>
			</Datas>
			<GroupsData>
				<Group groupName="MAT" dynamic="false" supportsInstance="true" sourceObject="MATERIAL">
					<Datas>
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

				in vec2 frag_textureCoord;
				in mat3 frag_TBNMatrix;
				flat in int frag_index;

				layout(std140) uniform MAT
				{
					float Specular[20];
					float Roughness[20];
					float Metallic[20];
					float ReflectionIntensity[20];
				};

				uniform sampler2D AlbedoMap;
				uniform sampler2D NormalMap;

				out vec4 FragColor[4];

				vec3 calcNormal(vec2 textureCoords)
				{					
					vec3 bumpNormal = texture(NormalMap, textureCoords).xyz;
					bumpNormal = 2.0 * bumpNormal - vec3(1.0, 1.0, 1.0);	
					vec3 newNormal = frag_TBNMatrix * bumpNormal;
					newNormal = normalize(newNormal);
					
					return newNormal;
				}

				void main()
				{
					vec3 albedoColor = texture(AlbedoMap, frag_textureCoord).rgb;
					vec3 normal = calcNormal(frag_textureCoord);
					
					// gamma correction
					//albedoColor = pow(albedoColor.rgb, vec3(2.2f));
					
					// Lerp with metallic
					vec3 realAlbedo = albedoColor - (albedoColor * Metallic[frag_index]);
					
					float spec = 0.02 + Specular[frag_index] * 0.03;
					vec3 realSpecular = mix(vec3(spec), albedoColor, Metallic[frag_index]);
					
					FragColor[1] = vec4(realAlbedo, Roughness[frag_index]);
					FragColor[2] = vec4(normal * 0.5 + 0.5, ReflectionIntensity[frag_index]);
					FragColor[3] = vec4(realSpecular, Metallic[frag_index]);
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>