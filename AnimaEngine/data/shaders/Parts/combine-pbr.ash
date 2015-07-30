<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader name="combine-pbr-fs" type="FS">
	<Part api="OGL" minVersion="3.3">
		<ShaderCode>
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
				FragColor.xyz += 	light;
				FragColor.xyz += 	specular;
				FragColor.a =		1.0f;
			}
			]]>
		</ShaderCode>
	</Part>
</AnimaShader>