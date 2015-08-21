<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader name="dil-shadow-map-fs" type="FS">
	<Part api="OGL" minVersion="3.3">
		<ShaderCode>
			<![CDATA[
			#version 150 core

			out vec4 FragColor;

			void main()
			{
				//VAR float depth 	= gl_FragCoord.z;
				//VAR float dx 		= dFdx(depth);
				//VAR float dy 		= dFdy(depth);
				//VAR float moment2 	= depth * depth + 0.25f * (dx * dx + dy * dy);

				//VAR FragColor = vec4(depth, moment2, 0.0f, 0.0f);
				FragColor = vec4(1.0f);
			}
			]]>
		</ShaderCode>
	</Part>
</AnimaShader>