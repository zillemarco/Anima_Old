#version 410

out vec4 fragColor;

uniform vec2 REN_ScreenSize;
uniform sampler2D REN_GB_PrepassBuffer_AlbedoMap;

uniform vec3 _ambientLight;

vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / REN_ScreenSize;
}

void main()
{
	vec2 textCoord = CalcTexCoord();
	vec4 color = texture(REN_GB_PrepassBuffer_AlbedoMap, textCoord);
	
	fragColor = color * vec4(_ambientLight, 1.0);
}
