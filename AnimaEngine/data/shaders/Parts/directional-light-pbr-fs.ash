<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>directional-light-pbr-fs</Name>
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
				<Data name="REN_DILShadowMapTexelSize" type="FLOAT2" />
				<Data name="REN_DILShadowMap" type="TEXTURE2D" />
				<Data name="CAM_Position" type="FLOAT3" />
				<Data name="CAM_InverseProjectionViewMatrix" type="MATRIX4x4" />
				<Data name="DIL_Direction" type="FLOAT3" />
				<Data name="DIL_Color" type="FLOAT3" />
				<Data name="DIL_Intensity" type="FLOAT" />
				<Data name="DIL_ProjectionViewMatrix" type="MATRIX4x4" />
				<Data name="DIL_ShadowMapBias" type="FLOAT" />
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

				uniform vec2 REN_DILShadowMapTexelSize;
				uniform sampler2D REN_DILShadowMap;

				uniform vec3 CAM_Position;
				uniform mat4 CAM_InverseProjectionViewMatrix;

				uniform vec3 DIL_Direction;
				uniform vec3 DIL_Color;
				uniform float DIL_Intensity;
				uniform mat4 DIL_ProjectionViewMatrix;
				uniform float DIL_ShadowMapBias;

				#include "pbr-functions"

				float ComputeShadowAmount(sampler2D shadowMap, vec2 coords, float compare)
				{
					vec2 pixelPos = coords / REN_DILShadowMapTexelSize + vec2(0.5f);
					vec2 fractPart = fract(pixelPos);
					vec2 startTexel = (pixelPos - fractPart) * REN_DILShadowMapTexelSize;

					float bl = step(compare, texture(shadowMap, startTexel).r + DIL_ShadowMapBias);
					float br = step(compare, texture(shadowMap, startTexel + vec2(REN_DILShadowMapTexelSize.x, 0.0f)).r + DIL_ShadowMapBias);
					float tl = step(compare, texture(shadowMap, startTexel + vec2(0.0f, REN_DILShadowMapTexelSize.t)).r + DIL_ShadowMapBias);
					float tr = step(compare, texture(shadowMap, startTexel + REN_DILShadowMapTexelSize).r + DIL_ShadowMapBias);

					float mixA = mix(bl, tl, fractPart.y);
					float mixB = mix(br, tr, fractPart.y);

					return mix(mixA, mixB, fractPart.x);
				}

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

					vec4 shadowCoord 	= DIL_ProjectionViewMatrix * vec4(pos, 1.0f);
					shadowCoord 		/= shadowCoord.w;
					shadowCoord.xyz		= shadowCoord.xyz * vec3(0.5f, 0.5f, 0.5f) + vec3(0.5f, 0.5f, 0.5f);

					float shadowAmount 	= ComputeShadowAmount(REN_DILShadowMap, shadowCoord.xy, shadowCoord.z);

					vec3 albedoColor 			= albedoData.xyz;
					vec3 specularColor 			= specularData.xyz;
					float roughness				= albedoData.w;
					float metallic				= specularData.w;
					vec3 viewDir 				= normalize(CAM_Position - pos);
					float reflectionIntensity 	= normalData.w;
					
					// Colore dell'ambiente
					vec3 reflectVector = reflect( -viewDir, normal);
					float mipIndex =  roughness * roughness * 8.0f;

				    vec3 envColor = textureLod(REN_EnvironmentMap, reflectVector, mipIndex).rgb;
				    //vec3 envColor = texture(REN_EnvironmentMap, reflectVector).rgb;
				    envColor = pow(envColor.rgb, vec3(2.2f));

				    vec3 irradiance = texture(REN_IrradianceMap, normal).rgb;

					vec3 luce = ComputeLight(albedoColor, specularColor, normal, roughness, DIL_Color, -DIL_Direction, viewDir) * DIL_Intensity;
					vec3 fresnel = Specular_F_Roughness(specularColor, roughness * roughness, normal, viewDir) * envColor * reflectionIntensity;
					
					// luce
					FragColor[0] = vec4(luce, 1.0f) * shadowAmount;
					
					// fresnel
					FragColor[1] = vec4(fresnel, 1.0f) * shadowAmount;

					// irradiance
					FragColor[2] = vec4(irradiance, 1.0f) * shadowAmount;
				}

				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>