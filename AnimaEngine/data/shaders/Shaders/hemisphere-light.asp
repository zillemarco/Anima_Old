<?xml version="1.0" encoding="UTF-8"?>
<AnimaShaderProgram name="hemisphere-light">
	<Shaders>
		<Shader name="hemisphere-light-fs" />
		<Shader name="hemisphere-light-vs" />
	</Shaders>
	<Datas>
		<Data name="MOD_AModelMatrix" type="MATRIX4x4" />
		<Data name="CAM_InverseProjectionViewMatrix" type="MATRIX4x4" />
		<Data name="REN_GB_PrepassBuffer_DepthMap" type="TEXTURE2D" />
		<Data name="REN_GB_PrepassBuffer_NormalMap" type="TEXTURE2D" />
		<Data name="REN_InverseScreenSize" type="FLOAT2" />
		<Data name="HEL_Position" type="FLOAT3" />
		<Data name="HEL_SkyColor" type="FLOAT3" />
		<Data name="HEL_GroundColor" type="FLOAT3" />
		<Data name="HEL_Intensity" type="FLOAT" />
	</Datas>
</AnimaShaderProgram>