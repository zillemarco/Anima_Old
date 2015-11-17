<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>point-light-pbr-fs</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data propertyName="PrepassBuffer.AlbedoMap" type="TEXTURE2D" sourceObject="GBUFFER" associatedWith="PrepassBuffer_AlbedoMap"/>
				<Data propertyName="PrepassBuffer.DepthMap" type="TEXTURE2D" sourceObject="GBUFFER" associatedWith="PrepassBuffer_DepthMap"/>
				<Data propertyName="PrepassBuffer.NormalMap" type="TEXTURE2D" sourceObject="GBUFFER" associatedWith="PrepassBuffer_NormalMap"/>
				<Data propertyName="PrepassBuffer.SpecularMap" type="TEXTURE2D" sourceObject="GBUFFER" associatedWith="PrepassBuffer_SpecularMap"/>
				<Data propertyName="EnvironmentMap" type="TEXTURECUBE" sourceObject="RENDERER"/>
				<Data propertyName="IrradianceMap" type="TEXTURECUBE" sourceObject="RENDERER"/>
				<Data propertyName="InverseScreenSize" type="FLOAT2" sourceObject="RENDERER"/>
				<Data propertyName="Position" type="FLOAT3" sourceObject="CAMERA" associatedWith="CameraPosition"/>
				<Data propertyName="InverseProjectionViewMatrix" type="MATRIX4x4" sourceObject="CAMERA"/>
				<Data propertyName="Range" type="FLOAT" sourceObject="LIGHT"/>
				<Data propertyName="Position" type="FLOAT3" sourceObject="LIGHT" associatedWith="LightPosition"/>
				<Data propertyName="Color" type="FLOAT3" sourceObject="LIGHT"/>
				<Data propertyName="Intensity" type="FLOAT" sourceObject="LIGHT"/>
				<Data propertyName="ConstantAttenuation" type="FLOAT" sourceObject="LIGHT"/>
				<Data propertyName="LinearAttenuation" type="FLOAT" sourceObject="LIGHT"/>
				<Data propertyName="ExponentAttenuation" type="FLOAT" sourceObject="LIGHT"/>
			</Datas>
			<Code>
				<![CDATA[
				#version 150 core

				out vec4 FragColor[3];

				uniform sampler2D PrepassBuffer_DepthMap;
				uniform sampler2D PrepassBuffer_NormalMap;
				uniform sampler2D PrepassBuffer_SpecularMap;
				uniform sampler2D PrepassBuffer_AlbedoMap;
				uniform samplerCube EnvironmentMap;
				uniform samplerCube IrradianceMap;
				uniform vec2 InverseScreenSize;

				uniform vec3 CameraPosition;
				uniform mat4 InverseProjectionViewMatrix;

				uniform float Range;
				uniform vec3 LightPosition;
				uniform vec3 Color;
				uniform float Intensity;
				uniform float ConstantAttenuation;
				uniform float LinearAttenuation;
				uniform float ExponentAttenuation;
				
				#include "pbr-functions"

				void main()
				{
					vec3 genPos	= vec3((gl_FragCoord.x * InverseScreenSize.x), (gl_FragCoord.y * InverseScreenSize.y), 0.0f);
					genPos.z 	= texture(PrepassBuffer_DepthMap, genPos.xy).r;

					vec4 normalData 	= texture(PrepassBuffer_NormalMap, genPos.xy);
					vec4 specularData 	= texture(PrepassBuffer_SpecularMap, genPos.xy);
					vec4 albedoData 	= texture(PrepassBuffer_AlbedoMap, genPos.xy);
					vec3 normal 		= normalize(normalData.xyz * 2.0f - 1.0f);
					vec4 clip 			= InverseProjectionViewMatrix * vec4(genPos * 2.0f - 1.0f, 1.0f);
					vec3 pos 			= clip.xyz / clip.w;

					float dist 	= length(LightPosition - pos);

					if(dist > Range)
					{
						discard;
					}

					vec3 albedoColor 			= albedoData.xyz;
					vec3 specularColor 			= specularData.xyz;
					float roughness				= albedoData.w;
					float metallic				= specularData.w;
					vec3 viewDir 				= normalize(CameraPosition - pos);
					float reflectionIntensity 	= normalData.w;

					float atten 				= (ConstantAttenuation + LinearAttenuation * dist + ExponentAttenuation * dist * dist + 0.00001);
					vec3 lightDirection			= normalize(pos - LightPosition);

					// Colore dell'ambiente
					vec3 reflectVector = reflect( -viewDir, normal);
					float mipIndex =  roughness * roughness * 8.0f;

				    vec3 envColor = textureLod(EnvironmentMap, reflectVector, mipIndex).rgb;
				    //vec3 envColor = texture(EnvironmentMap, reflectVector).rgb;
				    envColor = pow(envColor.rgb, vec3(2.2f));

				    vec3 irradiance = texture(IrradianceMap, normal).rgb;

					vec3 luce = ComputeLight(albedoColor, specularColor, normal, roughness, Color, -lightDirection, viewDir) * Intensity / atten;
					vec3 fresnel = Specular_F_Roughness(specularColor, roughness * roughness, normal, viewDir) * envColor * reflectionIntensity;
					
					// luce
					FragColor[0] = vec4(luce, 1.0f);
					
					// fresnel
					FragColor[1] = vec4(fresnel, 1.0f);

					// irradiance
					FragColor[2] = vec4(irradiance, 1.0f);
				}

				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>