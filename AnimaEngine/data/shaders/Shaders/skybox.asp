<?xml version="1.0" encoding="UTF-8"?>
<AnimaShaderProgram name="skybox">
	<Shaders>
		<Shader name="skybox-vs" />
		<Shader name="skybox-fs" />
	</Shaders>
	<Datas>
		<Data name="CAM_ProjectionViewMatrix" type="MATRIX4x4" />
		<Data name="MOD_AModelMatrix" type="MATRIX4x4" />
		<Data name="REN_SkyBox" type="TEXTURECUBE" />
	</Datas>
</AnimaShaderProgram>