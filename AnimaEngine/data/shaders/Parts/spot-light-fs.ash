<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>spot-light-fs</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data name="REN_GB_PrepassBuffer_DepthMap" type="TEXTURE2D" />
				<Data name="REN_GB_PrepassBuffer_NormalMap" type="TEXTURE2D" />
				<Data name="REN_GB_PrepassBuffer_SpecularMap" type="TEXTURE2D" />
				<Data name="REN_InverseScreenSize" type="FLOAT2" />
				<Data name="CAM_Position" type="FLOAT3" />
				<Data name="SPL_Range" type="FLOAT" />
				<Data name="SPL_Position" type="FLOAT3" />
				<Data name="SPL_Color" type="FLOAT3" />
				<Data name="SPL_Intensity" type="FLOAT" />
				<Data name="SPL_ConstantAttenuation" type="FLOAT" />
				<Data name="SPL_LinearAttenuation" type="FLOAT" />
				<Data name="SPL_ExponentAttenuation" type="FLOAT" />
				<Data name="SPL_Cutoff" type="FLOAT" />
				<Data name="SPL_Direction" type="FLOAT3" />
			</Datas>
			<Code>
				<![CDATA[
				#version 150 core

				in mat4 frag_inverseProjectionViewMatrix;
				out vec4 FragColor[2];

				uniform sampler2D REN_GB_PrepassBuffer_DepthMap;
				uniform sampler2D REN_GB_PrepassBuffer_NormalMap;
				uniform sampler2D REN_GB_PrepassBuffer_SpecularMap;
				uniform vec2 REN_InverseScreenSize;
				uniform vec3 CAM_Position;

				uniform float SPL_Range;
				uniform vec3 SPL_Position;
				uniform vec3 SPL_Color;
				uniform float SPL_Intensity;
				uniform float SPL_ConstantAttenuation;
				uniform float SPL_LinearAttenuation;
				uniform float SPL_ExponentAttenuation;
				uniform float SPL_Cutoff;
				uniform vec3 SPL_Direction;

				void main()
				{
					vec3 pos 	= vec3((gl_FragCoord.x * REN_InverseScreenSize.x), (gl_FragCoord.y * REN_InverseScreenSize.y), 0.0f);
					pos.z 		= texture(REN_GB_PrepassBuffer_DepthMap, pos.xy).r;

					vec3 normal 		= normalize(texture(REN_GB_PrepassBuffer_NormalMap, pos.xy).xyz * 2.0f - 1.0f);
					vec4 speclarData 	= texture(REN_GB_PrepassBuffer_SpecularMap, pos.xy);
					vec4 clip 			= frag_inverseProjectionViewMatrix * vec4(pos * 2.0f - 1.0f, 1.0f);
					pos 				= clip.xyz / clip.w;

					vec3 direction = SPL_Position - pos;
					float dist 	= length(direction);

					direction = normalize(direction);
					float spotFactor = dot(direction, -SPL_Direction);

					if(dist > SPL_Range || spotFactor <= SPL_Cutoff)
					{
						discard;
					}

					float spotAttenuation = (1.0 - ((1.0 - spotFactor) / (1.0 - SPL_Cutoff)));

					vec3 specularColor 	= speclarData.xyz;
					float shininess 	= 1.0f / speclarData.a;
					float atten 		= (SPL_ConstantAttenuation + SPL_LinearAttenuation * dist +  SPL_ExponentAttenuation * dist * dist + 0.00001);

					vec3 incident 	= direction;
					vec3 viewDir 	= normalize(CAM_Position - pos);
					vec3 halfDir 	= normalize(incident + viewDir);

					float lambert 	= clamp(dot(incident, normal), 0.0f, 1.0f);
					float rFactor 	= clamp(dot(halfDir, normal), 0.0f, 1.0f);
					float sFactor 	= pow(rFactor, shininess);
					vec3 sColor 	= specularColor * sFactor;

					FragColor[0] = vec4(SPL_Color * lambert * SPL_Intensity / atten, 1.0f);
					FragColor[1] = vec4(SPL_Color * sColor * SPL_Intensity / atten, 1.0f);
				}

				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>