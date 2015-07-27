<?xml version="1.0" encoding="UTF-8"?>
<AnimaShaderProgram name="deferred-prepare">
	<Shaders>
		<Shader name="static-mesh-vs" />
		<Shader name="base-material-fs" />
	</Shaders>
	<Datas>
		<Data name="CAM_ProjectionViewMatrix" type="MATRIX4x4" />
		<Data name="MOD_AModelMatrix" type="MATRIX4x4" />
		<Data name="MOD_ANormalMatrix" type="MATRIX4x4" />
		<Data name="MAT_DiffuseColor" type="FLOAT4" />
		<Data name="MAT_SpecularColor" type="FLOAT3" />
		<Data name="MAT_Shininess" type="FLOAT" />
	</Datas>
</AnimaShaderProgram>