<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>point-light-pbr-fs</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data name="REN_GB_PrepassBuffer_AlbedoMap" type="TEXTURE2D" />
				<Data name="REN_GB_PrepassBuffer_DepthMap" type="TEXTURE2D" />
				<Data name="REN_GB_PrepassBuffer_NormalMap" type="TEXTURE2D" />
				<Data name="REN_GB_PrepassBuffer_SpecularMap" type="TEXTURE2D" />
				<Data name="REN_EnvironmentMap" type="TEXTURECUBE" />
				<Data name="REN_IrradianceMap" type="TEXTURECUBE" />
				<Data name="REN_InverseScreenSize" type="FLOAT2" />
				<Data name="CAM_Position" type="FLOAT3" />
				<Data name="CAM_InverseProjectionViewMatrix" type="MATRIX4x4" />
				<Data name="PTL_Range" type="FLOAT" />
				<Data name="PTL_Position" type="FLOAT3" />
				<Data name="PTL_Color" type="FLOAT3" />
				<Data name="PTL_Intensity" type="FLOAT" />
				<Data name="PTL_ConstantAttenuation" type="FLOAT" />
				<Data name="PTL_LinearAttenuation" type="FLOAT" />
				<Data name="PTL_ExponentAttenuation" type="FLOAT" />
			</Datas>
			<Code>
				<![CDATA[
				#version 150 core

				out vec4 FragColor[3];

				uniform sampler2D REN_GB_PrepassBuffer_DepthMap;
				uniform sampler2D REN_GB_PrepassBuffer_NormalMap;
				uniform sampler2D REN_GB_PrepassBuffer_SpecularMap;
				uniform sampler2D REN_GB_PrepassBuffer_AlbedoMap;
				uniform samplerCube REN_EnvironmentMap;
				uniform samplerCube REN_IrradianceMap;
				uniform vec2 REN_InverseScreenSize;

				uniform vec3 CAM_Position;
				uniform mat4 CAM_InverseProjectionViewMatrix;

				uniform float PTL_Range;
				uniform vec3 PTL_Position;
				uniform vec3 PTL_Color;
				uniform float PTL_Intensity;
				uniform float PTL_ConstantAttenuation;
				uniform float PTL_LinearAttenuation;
				uniform float PTL_ExponentAttenuation;
				
				#include "pbr-functions"

				void main()
				{
					vec3 genPos	= vec3((gl_FragCoord.x * REN_InverseScreenSize.x), (gl_FragCoord.y * REN_InverseScreenSize.y), 0.0f);
					genPos.z 	= texture(REN_GB_PrepassBuffer_DepthMap, genPos.xy).r;

					vec4 normalData 	= texture(REN_GB_PrepassBuffer_NormalMap, genPos.xy);
					vec4 specularData 	= texture(REN_GB_PrepassBuffer_SpecularMap, genPos.xy);
					vec4 albedoData 	= texture(REN_GB_PrepassBuffer_AlbedoMap, genPos.xy);
					vec3 normal 		= normalize(normalData.xyz * 2.0f - 1.0f);
					vec4 clip 			= CAM_InverseProjectionViewMatrix * vec4(genPos * 2.0f - 1.0f, 1.0f);
					vec3 pos 			= clip.xyz / clip.w;

					float dist 	= length(PTL_Position - pos);

					if(dist > PTL_Range)
					{
						discard;
					}

					vec3 albedoColor 			= albedoData.xyz;
					vec3 specularColor 			= specularData.xyz;
					float roughness				= albedoData.w;
					float metallic				= specularData.w;
					vec3 viewDir 				= normalize(CAM_Position - pos);
					float reflectionIntensity 	= normalData.w;

					float atten 				= (PTL_ConstantAttenuation + PTL_LinearAttenuation * dist +  PTL_ExponentAttenuation * dist * dist + 0.00001);
					vec3 lightDirection			= normalize(pos - PTL_Position);

					// Colore dell'ambiente
					vec3 reflectVector = reflect( -viewDir, normal);
					float mipIndex =  roughness * roughness * 8.0f;

				    vec3 envColor = textureLod(REN_EnvironmentMap, reflectVector, mipIndex).rgb;
				    //vec3 envColor = texture(REN_EnvironmentMap, reflectVector).rgb;
				    envColor = pow(envColor.rgb, vec3(2.2f));

				    vec3 irradiance = texture(REN_IrradianceMap, normal).rgb;

					vec3 luce = ComputeLight(albedoColor, specularColor, normal, roughness, PTL_Color, -lightDirection, viewDir) * PTL_Intensity / atten;
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