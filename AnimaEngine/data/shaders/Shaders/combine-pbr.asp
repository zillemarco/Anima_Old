<?xml version="1.0" encoding="UTF-8"?>
<AnimaShaderProgram name="deferred-combine">
	<Shaders>
		<Shader name="generic-filter-vs" />
		<Shader name="combine-pbr-fs" />
	</Shaders>
	<Datas>
		<Data name="CAM_ProjectionViewMatrix" type="MATRIX4x4" />
		<Data name="MOD_AModelMatrix" type="MATRIX4x4" />
		<Data name="REN_GB_PrepassBuffer_AlbedoMap" type="TEXTURE2D" />
		<Data name="REN_GB_LightsBuffer_EmissiveMap" type="TEXTURE2D" />
		<Data name="REN_GB_LightsBuffer_SpecularMap" type="TEXTURE2D" />
		<Data name="REN_AmbientLight" type="FLOAT3" />
		<Data name="REN_ScreenSize" type="FLOAT2" />
	</Datas>
</AnimaShaderProgram>