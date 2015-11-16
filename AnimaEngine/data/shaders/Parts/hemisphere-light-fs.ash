<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>hemisphere-light-fs</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data propertyName="InverseProjectionViewMatrix" type="MATRIX4x4" sourceObect="CAMERA"/>
				<Data propertyName="PrepassBuffer" type="TEXTURE2D" sourceObect="GBUFFER" slot="DepthMap" associatedWith="PrepassBuffer_DepthMap"/>
				<Data propertyName="PrepassBuffer" type="TEXTURE2D" sourceObect="GBUFFER" slot="NormalMap" associatedWith="PrepassBuffer_NormalMap"/>
				<Data propertyName="InverseScreenSize" type="FLOAT2" sourceObect="RENDERER"/>
				<Data propertyName="Position" type="FLOAT3" sourceObect="LIGHT"/>
				<Data propertyName="SkyColor" type="FLOAT3" sourceObect="LIGHT"/>
				<Data propertyName="GroundColor" type="FLOAT3" sourceObect="LIGHT"/>
				<Data propertyName="Intensity" type="FLOAT" sourceObect="LIGHT"/>
			</Datas>
			<Code>
				<![CDATA[
				#version 150 core

				out vec4 FragColor[2];

				uniform mat4 InverseProjectionViewMatrix;

				uniform sampler2D PrepassBuffer_DepthMap;
				uniform sampler2D PrepassBuffer_NormalMap;
				uniform vec2 InverseScreenSize;

				uniform vec3 Position;
				uniform vec3 SkyColor;
				uniform vec3 GroundColor;
				uniform float Intensity;

				void main()
				{
					vec3 pos 	= vec3((gl_FragCoord.x * InverseScreenSize.x), (gl_FragCoord.y * InverseScreenSize.y), 0.0f);
					pos.z 		= texture(PrepassBuffer_DepthMap, pos.xy).r;

					vec3 normal 		= normalize(texture(PrepassBuffer_NormalMap, pos.xy).xyz * 2.0f - 1.0f);
					vec4 clip 			= InverseProjectionViewMatrix * vec4(pos * 2.0f - 1.0f, 1.0f);
					pos 				= clip.xyz / clip.w;

					vec3 lightVec 	= normalize(Position - pos);
					float cosTheta 	= dot(normal, lightVec);
					float a 		= cosTheta * 0.5f + 0.5f;
					vec3 lightColor = mix(GroundColor, SkyColor, a);

					FragColor[0] = vec4(lightColor * Intensity, 1.0f);
					FragColor[1] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>