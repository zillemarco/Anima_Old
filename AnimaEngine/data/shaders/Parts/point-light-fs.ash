<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>point-light-fs</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data propertyName="PrepassBuffer" type="TEXTURE2D" sourceObject="GBUFFER" slot="DepthMap" associatedWith="PrepassBuffer_DepthMap"/>
				<Data propertyName="PrepassBuffer" type="TEXTURE2D" sourceObject="GBUFFER" slot="NormalMap" associatedWith="PrepassBuffer_NormalMap"/>
				<Data propertyName="PrepassBuffer" type="TEXTURE2D" sourceObject="GBUFFER" slot="SpecularMap" associatedWith="PrepassBuffer_SpecularMap"/>
				<Data propertyName="InverseScreenSize" type="FLOAT2" sourceObject="RENDERER"/>
				<Data propertyName="Position" type="FLOAT3" sourceObject="CAMERA" associatedWith="CameraPosition"/>
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

				in mat4 frag_inverseProjectionViewMatrix;
				out vec4 FragColor[2];

				uniform sampler2D PrepassBuffer_DepthMap;
				uniform sampler2D PrepassBuffer_NormalMap;
				uniform sampler2D PrepassBuffer_SpecularMap;
				uniform vec2 InverseScreenSize;
				uniform vec3 CameraPosition;

				uniform float Range;
				uniform vec3 LightPosition;
				uniform vec3 Color;
				uniform float Intensity;
				uniform float ConstantAttenuation;
				uniform float LinearAttenuation;
				uniform float ExponentAttenuation;

				void main()
				{
					vec3 pos 	= vec3((gl_FragCoord.x * InverseScreenSize.x), (gl_FragCoord.y * InverseScreenSize.y), 0.0f);
					pos.z 		= texture(PrepassBuffer_DepthMap, pos.xy).r;

					vec3 normal 		= normalize(texture(PrepassBuffer_NormalMap, pos.xy).xyz * 2.0f - 1.0f);
					vec4 speclarData 	= texture(PrepassBuffer_SpecularMap, pos.xy);
					vec4 clip 			= frag_inverseProjectionViewMatrix * vec4(pos * 2.0f - 1.0f, 1.0f);
					pos 				= clip.xyz / clip.w;

					float dist 	= length(LightPosition - pos);

					if(dist > Range)
					{
						discard;
					}

					vec3 specularColor 	= speclarData.xyz;
					float shininess 	= 1.0f / speclarData.a;
					float atten 		= (ConstantAttenuation + LinearAttenuation * dist + ExponentAttenuation * dist * dist + 0.00001);

					vec3 incident 	= normalize(LightPosition - pos);
					vec3 viewDir 	= normalize(CameraPosition - pos);
					vec3 halfDir 	= normalize(incident + viewDir);

					float lambert 	= clamp(dot(incident, normal), 0.0f, 1.0f);
					float rFactor 	= clamp(dot(halfDir, normal), 0.0f, 1.0f);
					float sFactor 	= pow(rFactor, shininess);
					vec3 sColor 	= specularColor * sFactor;

					FragColor[0] = vec4(Color * lambert * Intensity / atten, 1.0f);
					FragColor[1] = vec4(Color * sColor * Intensity / atten, 1.0f);
				}

				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>