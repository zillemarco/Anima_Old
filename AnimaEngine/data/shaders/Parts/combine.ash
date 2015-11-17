<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>combine-fs</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data propertyName="PrepassBuffer.AlbedoMap" type="TEXTURE2D" sourceObject="GBUFFER" associatedWith="PrepassBuffer_AlbedoMap"/>
				<Data propertyName="LightsBuffer.EmissiveMap" type="TEXTURE2D" sourceObject="GBUFFER" associatedWith="LightsBuffer_EmissiveMap"/>
				<Data propertyName="LightsBuffer.SpecularMap" type="TEXTURE2D" sourceObject="GBUFFER" associatedWith="LightsBuffer_SpecularMap"/>
				<Data propertyName="AmbientLight" type="FLOAT3" sourceObject="RENDERER"/>
				<Data propertyName="ScreenSize" type="FLOAT2" sourceObject="RENDERER"/>
			</Datas>
			<Code>
				<![CDATA[
				#version 150 core

				uniform sampler2D PrepassBuffer_AlbedoMap;
				uniform sampler2D LightsBuffer_EmissiveMap;
				uniform sampler2D LightsBuffer_SpecularMap;
				uniform vec3 AmbientLight;
				uniform vec2 ScreenSize;

				vec2 CalcTexCoord()
				{
				    return gl_FragCoord.xy / ScreenSize;
				}

				out vec4 FragColor;

				void main()
				{
					vec2 textureCoord = CalcTexCoord();

					vec3 diffuse 	= texture(PrepassBuffer_AlbedoMap, textureCoord).xyz;
					vec3 light 		= texture(LightsBuffer_EmissiveMap, textureCoord).xyz;
					vec3 specular 	= texture(LightsBuffer_SpecularMap, textureCoord).xyz;

					FragColor.xyz = 	diffuse * AmbientLight;
					FragColor.xyz += 	diffuse * light;
					FragColor.xyz += 	specular;
					FragColor.a =		1.0f;
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>