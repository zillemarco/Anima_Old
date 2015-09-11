<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>dil-shadow-map-fs-inst-vsm</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas></Datas>
			<Code>
				<![CDATA[
				#version 150 core

				out vec4 FragColor;

				void main()
				{
					float depth = gl_FragCoord.z;
					float dx = dFdx(depth);
					float dy = dFdy(depth);
					float moment2 = depth * depth + 0.25 * (dx * dx + dy * dy);

					FragColor = vec4(depth, moment2, 0.0, 1.0);
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>