<?xml version="1.0" encoding="UTF-8"?>
<AnimaShaderProgram name="nullFilter">
	<Shaders>
		<Shader name="generic-filter-vs" />
		<Shader name="nullFilter-fs" />
	</Shaders>
	<Datas>
		<Data name="CAM_ProjectionViewMatrix" type="MATRIX4x4" />
		<Data name="MOD_AModelMatrix" type="MATRIX4x4" />
		<Data name="REN_FilterMap" type="TEXTURE" />
		<Data name="REN_ScreenSize" type="FLOAT2" />
	</Datas>
</AnimaShaderProgram>