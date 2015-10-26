<?xml version="1.0" encoding="UTF-8"?>
<AnimaShader>
	<Name>physicsDebug-fs</Name>
	<Type>FS</Type>
	<Versions>
		<Version>
			<API>OGL</API>
			<MinVersion>3.3</MinVersion>
			<Datas/>
			<Code>
				<![CDATA[
				#version 150 core

				in vec3 frag_color;
				out vec4 FragColor;

				void main()
				{          
				    FragColor = vec4(frag_color, 1.0f);
				}
				]]>
			</Code>
		</Version>
	</Versions>
</AnimaShader>