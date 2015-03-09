#version 150 core

uniform sampler2D REN_FilterMap;
uniform vec2 REN_TextureSize;
uniform vec3 REN_BlurScale;

vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / REN_TextureSize;
}

out vec4 FragColor;

void main()
{
    vec2 textCoord = CalcTexCoord();
	vec4 color = vec4(0.0f);

	color += texture(REN_FilterMap, textCoord + (vec2(-3.0f) * REN_BlurScale.xy)) * (1.0f / 64.0f);
	color += texture(REN_FilterMap, textCoord + (vec2(-2.0f) * REN_BlurScale.xy)) * (6.0f / 64.0f);
	color += texture(REN_FilterMap, textCoord + (vec2(-1.0f) * REN_BlurScale.xy)) * (15.0f / 64.0f);
	color += texture(REN_FilterMap, textCoord + (vec2(0.0f)  * REN_BlurScale.xy)) * (20.0f / 64.0f);
	color += texture(REN_FilterMap, textCoord + (vec2(1.0f)  * REN_BlurScale.xy)) * (15.0f / 64.0f);
	color += texture(REN_FilterMap, textCoord + (vec2(2.0f)  * REN_BlurScale.xy)) * (6.0f / 64.0f);
	color += texture(REN_FilterMap, textCoord + (vec2(3.0f)  * REN_BlurScale.xy)) * (1.0f / 64.0f);

	FragColor = color;
}
