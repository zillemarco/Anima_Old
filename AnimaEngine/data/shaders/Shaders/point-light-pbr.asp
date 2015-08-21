<?xml version="1.0" encoding="UTF-8"?>
<AnimaShaderProgram name="point-light">
	<Shaders>
		<Shader name="point-light-pbr-fs" />
		<Shader name="point-light-vs" />
	</Shaders>
	<Datas>
		<Data name="MOD_AModelMatrix" type="MATRIX4x4" />
		<Data name="REN_GB_PrepassBuffer_AlbedoMap" type="TEXTURE2D" />
		<Data name="REN_GB_PrepassBuffer_DepthMap" type="TEXTURE2D" />
		<Data name="REN_GB_PrepassBuffer_NormalMap" type="TEXTURE2D" />
		<Data name="REN_GB_PrepassBuffer_SpecularMap" type="TEXTURE2D" />
		<Data name="REN_EnvironmentMap" type="TEXTURECUBE" />
		<Data name="REN_IrradianceMap" type="TEXTURECUBE" />
		<Data name="REN_InverseScreenSize" type="FLOAT2" />
		<Data name="CAM_Position" type="FLOAT3" />
		<Data name="CAM_InverseProjectionViewMatrix" type="MATRIX4x4" />
		<Data name="CAM_ProjectionViewMatrix" type="MATRIX4x4" />
		<Data name="PTL_Range" type="FLOAT" />
		<Data name="PTL_Position" type="FLOAT3" />
		<Data name="PTL_Color" type="FLOAT3" />
		<Data name="PTL_Intensity" type="FLOAT" />
		<Data name="PTL_ConstantAttenuation" type="FLOAT" />
		<Data name="PTL_LinearAttenuation" type="FLOAT" />
		<Data name="PTL_ExponentAttenuation" type="FLOAT" />
	</Datas>
</AnimaShaderProgram>