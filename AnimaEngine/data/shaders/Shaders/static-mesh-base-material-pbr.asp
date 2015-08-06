<?xml version="1.0" encoding="UTF-8"?>
<AnimaShaderProgram name="deferred-prepare">
	<Shaders>
		<Shader name="static-mesh-vs" />
		<Shader name="base-material-pbr-fs" />
	</Shaders>
	<Datas>
		<Data name="CAM_ProjectionViewMatrix" type="MATRIX4x4" />
		<Data name="MOD_AModelMatrix" type="MATRIX4x4" />
		<Data name="MOD_ANormalMatrix" type="MATRIX4x4" />
		<Data name="MAT_Albedo" type="FLOAT4" />
		<Data name="MAT_Roughness" type="FLOAT" />
		<Data name="MAT_Metallic" type="FLOAT" />
		<Data name="MAT_Specular" type="FLOAT4" />
		<Data name="MAT_ReflectionIntensity" type="FLOAT" />
	</Datas>
</AnimaShaderProgram>