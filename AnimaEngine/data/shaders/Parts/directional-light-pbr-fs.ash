<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>directional-light-pbr-fs</Name>
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
				<Data propertyName="EnvironmentMap" type="TEXTURECUBE" sourceObject="SCENE"/>
				<Data propertyName="IrradianceMap" type="TEXTURECUBE" sourceObject="SCENE"/>
				<Data propertyName="InverseScreenSize" type="FLOAT2" sourceObject="RENDERER"/>
				<Data propertyName="DILShadowMap" type="TEXTURE2D" sourceObject="RENDERER"/>
				<Data propertyName="Position" type="FLOAT3" sourceObject="CAMERA" associatedWith="CameraPosition"/>
				<Data propertyName="InverseProjectionViewMatrix" type="MATRIX4x4" sourceObject="CAMERA"/>
				<Data propertyName="Direction" type="FLOAT3" sourceObject="LIGHT"/>
				<Data propertyName="Color" type="FLOAT3" sourceObject="LIGHT"/>
				<Data propertyName="Intensity" type="FLOAT" sourceObject="LIGHT"/>
				<Data propertyName="ProjectionViewMatrix" type="MATRIX4x4" sourceObject="LIGHT"/>
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

				uniform sampler2D DILShadowMap;

				uniform vec3 CameraPosition;
				uniform mat4 InverseProjectionViewMatrix;

				uniform vec3 Direction;
				uniform vec3 Color;
				uniform float Intensity;
				uniform mat4 ProjectionViewMatrix;
				
				#include "pbr-functions"
				#include "compute-shadows-amount"

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

					vec4 shadowCoord 	= ProjectionViewMatrix * vec4(pos, 1.0f);
					shadowCoord 		/= shadowCoord.w;
					shadowCoord.xyz		= shadowCoord.xyz * vec3(0.5f, 0.5f, 0.5f) + vec3(0.5f, 0.5f, 0.5f);

					float shadowAmount 	= ComputeVarianceShadowAmount(DILShadowMap, shadowCoord.xy, shadowCoord.z);

					vec3 albedoColor 			= albedoData.xyz;
					vec3 specularColor 			= specularData.xyz;
					float roughness				= albedoData.w;
					float metallic				= specularData.w;
					vec3 viewDir 				= normalize(CameraPosition - pos);
					float reflectionIntensity 	= normalData.w;
					
					// Colore dell'ambiente
					vec3 reflectVector = reflect( -viewDir, normal);
					float mipIndex =  roughness * roughness * 8.0f;

				    vec3 envColor = textureLod(EnvironmentMap, reflectVector, mipIndex).rgb;
				    //vec3 envColor = texture(EnvironmentMap, reflectVector).rgb;
				    envColor = pow(envColor.rgb, vec3(2.2f));

				    vec3 irradiance = texture(IrradianceMap, normal).rgb;

					vec3 luce = ComputeLight(albedoColor, specularColor, normal, roughness, Color, -Direction, viewDir) * Intensity;
					vec3 fresnel = Specular_F_Roughness(specularColor, roughness * roughness, normal, viewDir) * envColor * reflectionIntensity;
					
					// luce
					FragColor[0] = vec4(1.0, 0.0, 0.0, 1.0);//vec4(luce, 1.0f) * shadowAmount;
					
					// fresnel
					FragColor[1] = vec4(0.0, 0.0, 1.0, 1.0);//vec4(fresnel, 1.0f) * shadowAmount;

					// irradiance
					FragColor[2] = vec4(0.0, 1.0, 0.0, 1.0);//vec4(irradiance, 1.0f) * shadowAmount;
				}

				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>