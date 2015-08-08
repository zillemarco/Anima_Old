<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader name="base-material-pbr-fs" type="FS">
	<Part api="OGL" minVersion="3.3">
		<ShaderCode>
			<![CDATA[
			#version 410 core

			in vec3 frag_normal;

			uniform vec4 MAT_Albedo;
			uniform vec4 MAT_Specular;
			uniform float MAT_Roughness;
			uniform float MAT_Metallic;
			uniform float MAT_ReflectionIntensity;

			out vec4 FragColor[4];

			void main()
			{
				vec3 albedoColor = MAT_Albedo.rgb;
				
				// gamma correction
				albedoColor = pow(albedoColor.rgb, vec3(2.2f));
				
				// Lerp with metallic
				vec3 realAlbedo = albedoColor - (albedoColor * MAT_Metallic);
				
				// 0.03 default value for dielectic
				vec3 realSpecular = mix(MAT_Specular.rgb, albedoColor, MAT_Metallic);
				
				FragColor[1] = vec4(realAlbedo, MAT_Roughness);
				FragColor[2] = vec4(frag_normal * 0.5 + 0.5, MAT_ReflectionIntensity);
				//FragColor[2] = vec4(frag_normal, 1.0);
				FragColor[3] = vec4(realSpecular, MAT_Metallic);
			}
			]]>
		</ShaderCode>
	</Part>
</AnimaShader>