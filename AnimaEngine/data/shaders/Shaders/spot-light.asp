<?xml version="1.0" encoding="UTF-8"?>
<AnimaShaderProgram name="spot-light">
	<Shaders>
		<Shader name="spot-light-fs" />
		<Shader name="spot-light-vs" />
	</Shaders>
	<Datas>
		<Data name="CAM_InverseProjectionViewMatrix" type="MATRIX4x4" />
		<Data name="CAM_ProjectionViewMatrix" type="MATRIX4x4" />
		<Data name="MOD_AModelMatrix" type="MATRIX4x4" />
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
</AnimaShaderProgram>