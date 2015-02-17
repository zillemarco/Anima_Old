#version 330

uniform sampler2D _renderingFilterMap;
uniform vec2 _renderingScreenSize;

vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / _renderingScreenSize;
}

out vec4 FragColor;

void main()
{
    vec2 TexCoord = CalcTexCoord();
	vec3 Color = texture(_renderingFilterMap, TexCoord).xyz;

	FragColor = vec4(Color, 1.0);
}
