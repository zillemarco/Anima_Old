<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader name="directional-light-pbr-fs" type="FS">
	<Part api="OGL" minVersion="3.3">
		<ShaderCode>
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

			uniform vec3 DIL_Direction;
			uniform vec3 DIL_Color;
			uniform float DIL_Intensity;
			uniform mat4 DIL_ProjectionViewMatrix;
			
			vec3 Diffuse(vec3 pAlbedo)
			{
				return pAlbedo / 3.14159265359f;
			}
			
			float NormalDistribution_GGX(float a, float NdH)
			{
				// Isotropic ggx.
				float a2 = a*a;
				float NdH2 = NdH * NdH;

				float denominator = NdH2 * (a2 - 1.0f) + 1.0f;
				denominator *= denominator;
				denominator *= 3.14159265359f;

				return a2 / denominator;
			}

			float NormalDistribution_BlinnPhong(float a, float NdH)
			{
				return (1 / (3.14159265359f * a * a)) * pow(NdH, 2 / (a * a) - 2);
			}

			float NormalDistribution_Beckmann(float a, float NdH)
			{
				float a2 = a * a;
				float NdH2 = NdH * NdH;

				return (1.0f/(3.14159265359f * a2 * NdH2 * NdH2 + 0.001)) * exp( (NdH2 - 1.0f) / ( a2 * NdH2));
			}
			
			float Geometric_Implicit(float a, float NdV, float NdL)
			{
				return NdL * NdV;
			}

			float Geometric_Neumann(float a, float NdV, float NdL)
			{
				return (NdL * NdV) / max(NdL, NdV);
			}

			float Geometric_CookTorrance(float a, float NdV, float NdL, float NdH, float VdH)
			{
				return min(1.0f, min((2.0f * NdH * NdV)/VdH, (2.0f * NdH * NdL)/ VdH));
			}

			float Geometric_Kelemen(float a, float NdV, float NdL, float LdV)
			{
				return (2 * NdL * NdV) / (1 + LdV);
			}

			float Geometric_Beckman(float a, float dotValue)
			{
				float c = dotValue / ( a * sqrt(1.0f - dotValue * dotValue));

				if ( c >= 1.6f )
				{
					return 1.0f;
				}
				else
				{
					float c2 = c * c;
					return (3.535f * c + 2.181f * c2) / ( 1 + 2.276f * c + 2.577f * c2);
				}
			}

			float Geometric_Smith_Beckmann(float a, float NdV, float NdL)
			{
				return Geometric_Beckman(a, NdV) * Geometric_Beckman(a, NdL);
			}

			float Geometric_GGX(float a, float dotValue)
			{
				float a2 = a * a;
				return (2.0f * dotValue) / (dotValue + sqrt(a2 + ((1.0f - a2) * (dotValue * dotValue))));
			}

			float Geometric_Smith_GGX(float a, float NdV, float NdL)
			{
				return Geometric_GGX(a, NdV) * Geometric_GGX(a, NdL);
			}

			float Geometric_Smith_Schlick_GGX(float a, float NdV, float NdL)
			{
				// Smith schlick-GGX.
				float k = a * 0.5f;
				float GV = NdV / (NdV * (1 - k) + k);
				float GL = NdL / (NdL * (1 - k) + k);

				return GV * GL;
			}
			
			vec3 Fresnel_None(vec3 specularColor)
			{
				return specularColor;
			}
			
			vec3 Fresnel_Schlick(vec3 specularColor, vec3 h, vec3 v)
			{
				return (specularColor + (1.0f - specularColor) * pow((1.0f - clamp(dot(v, h), 0.0f, 1.0f)), 5));
			}

			vec3 Fresnel_CookTorrance(vec3 specularColor, vec3 h, vec3 v)
			{
				vec3 n = (1.0f + sqrt(specularColor)) / (1.0f - sqrt(specularColor));
				float c = clamp(dot(v, h), 0.0f, 1.0f);
				vec3 g = sqrt(n * n + c * c - 1.0f);

				vec3 part1 = (g - c)/(g + c);
				vec3 part2 = ((g + c) * c - 1.0f)/((g - c) * c + 1.0f);

				vec3 nullValue = vec3(0.0, 0.0, 0.0);
				
				return max(nullValue, 0.5f * part1 * part1 * ( 1 + part2 * part2));
			}

			float Specular_D(float a, float NdH)
			{
				//return NormalDistribution_BlinnPhong(a, NdH);
				//return NormalDistribution_Beckmann(a, NdH);
				return NormalDistribution_GGX(a, NdH);
			}

			vec3 Specular_F(vec3 specularColor, vec3 h, vec3 v)
			{
				//return Fresnel_None(specularColor);
				return Fresnel_Schlick(specularColor, h, v);
				//return Fresnel_CookTorrance(specularColor, h, v);
			}

			vec3 Specular_F_Roughness(vec3 specularColor, float a, vec3 h, vec3 v)
			{
				// Sclick using roughness to attenuate fresnel.
				return (specularColor + (max(vec3(1.0f - a), specularColor) - specularColor) * pow((1 - clamp(dot(v, h), 0.0f, 1.0f)), 5));
				//return Fresnel_None(specularColor);
				//return Fresnel_CookTorrance(specularColor, h, v);
			}

			float Specular_G(float a, float NdV, float NdL, float NdH, float VdH, float LdV)
			{
				//return Geometric_Implicit(a, NdV, NdL);
				//return Geometric_Neumann(a, NdV, NdL);
				//return Geometric_CookTorrance(a, NdV, NdL, NdH, VdH);
				//return Geometric_Kelemen(a, NdV, NdL, LdV);
				//return Geometric_Smith_Beckmann(a, NdV, NdL);
				//return Geometric_Smith_GGX(a, NdV, NdL);
				return Geometric_Smith_Schlick_GGX(a, NdV, NdL);
			}

			vec3 Specular(vec3 specularColor, vec3 h, vec3 v, vec3 l, float a, float NdL, float NdV, float NdH, float VdH, float LdV)
			{
				return ((Specular_D(a, NdH) * Specular_G(a, NdV, NdL, NdH, VdH, LdV)) * Specular_F(specularColor, v, h) ) / (4.0f * NdL * NdV + 0.0001f);
			}

			vec3 ComputeLight(vec3 albedoColor, vec3 specularColor, vec3 normal, float roughness, vec3 lightColor, vec3 lightDir, vec3 viewDir)
			{
				float NdL = clamp(dot(normal, lightDir), 0.0, 1.0);
				float NdV = clamp(dot(normal, viewDir), 0.0, 1.0);
				vec3 h = normalize(lightDir + viewDir);
				float NdH = clamp(dot(normal, h), 0.0, 1.0);
				float VdH = clamp(dot(viewDir, h), 0.0, 1.0);
				float LdV = clamp(dot(lightDir, viewDir), 0.0, 1.0);
				float a = roughness * roughness;

				vec3 cDiff = Diffuse(albedoColor);
				vec3 cSpec = Specular(specularColor, h, viewDir, lightDir, a, NdL, NdV, NdH, VdH, LdV);

				return lightColor * NdL * (cDiff * (1.0f - cSpec) + cSpec);
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
				FragColor[0] = vec4(luce, 1.0f);
				
				// fresnel
				FragColor[1] = vec4(fresnel, 1.0f);

				// irradiance
				FragColor[2] = vec4(irradiance, 1.0f);
			}

			]]>
		</ShaderCode>
	</Part>
</AnimaShader>