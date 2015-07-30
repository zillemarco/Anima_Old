<?xml version="1.0" encoding="UTF-8"?>
<AnimaShaderProgram name="directional-light">
	<Shaders>
		<Shader name="directional-light-pbr-fs" />
		<Shader name="directional-light-vs" />
	</Shaders>
	<Datas>
		<Data name="MOD_AModelMatrix" type="MATRIX4x4" />
		<Data name="REN_GB_PrepassBuffer_DepthMap" type="TEXTURE2D" />
		<Data name="REN_GB_PrepassBuffer_NormalMap" type="TEXTURE2D" />
		<Data name="REN_GB_PrepassBuffer_SpecularMap" type="TEXTURE2D" />
		<Data name="REN_InverseScreenSize" type="FLOAT2" />
		<Data name="CAM_Position" type="FLOAT3" />
		<Data name="CAM_ProjectionViewInverseMatrix" type="MATRIX4x4" />
		<Data name="DIL_Direction" type="FLOAT3" />
		<Data name="DIL_Color" type="FLOAT3" />
		<Data name="DIL_Intensity" type="FLOAT" />
		<Data name="DIL_ProjectionViewMatrix" type="MATRIX4x4" />
	</Datas>
</AnimaShaderProgram>