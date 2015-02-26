#version 330

uniform sampler2D REN_FilterMap;
uniform vec2 REN_ScreenSize;

vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / REN_ScreenSize;
}

out vec4 FragColor;

void main()
{
    vec2 TexCoord = CalcTexCoord();
	vec3 Color = texture(REN_FilterMap, TexCoord).xyz;

	FragColor = vec4(Color, 1.0);
}
