<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>combine-fs</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas>
				<Data name="REN_GB_PrepassBuffer_AlbedoMap" type="TEXTURE2D" />
				<Data name="REN_GB_LightsBuffer_EmissiveMap" type="TEXTURE2D" />
				<Data name="REN_GB_LightsBuffer_SpecularMap" type="TEXTURE2D" />
				<Data name="REN_AmbientLight" type="FLOAT3" />
				<Data name="REN_ScreenSize" type="FLOAT2" />
			</Datas>
			<Code>
				<![CDATA[
				#version 150 core

				uniform sampler2D REN_GB_PrepassBuffer_AlbedoMap;
				uniform sampler2D REN_GB_LightsBuffer_EmissiveMap;
				uniform sampler2D REN_GB_LightsBuffer_SpecularMap;
				uniform vec3 REN_AmbientLight;
				uniform vec2 REN_ScreenSize;

				vec2 CalcTexCoord()
				{
				    return gl_FragCoord.xy / REN_ScreenSize;
				}

				out vec4 FragColor;

				void main()
				{
					vec2 textureCoord = CalcTexCoord();

					vec3 diffuse 	= texture(REN_GB_PrepassBuffer_AlbedoMap, textureCoord).xyz;
					vec3 light 		= texture(REN_GB_LightsBuffer_EmissiveMap, textureCoord).xyz;
					vec3 specular 	= texture(REN_GB_LightsBuffer_SpecularMap, textureCoord).xyz;

					FragColor.xyz = 	diffuse * REN_AmbientLight;
					FragColor.xyz += 	diffuse * light;
					FragColor.xyz += 	specular;
					FragColor.a =		1.0f;
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>