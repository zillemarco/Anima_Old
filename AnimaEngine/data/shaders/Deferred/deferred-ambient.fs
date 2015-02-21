#version 410

out vec4 fragColor;

uniform vec2 _renderingScreenSize;
uniform sampler2D _renderingDeferredAlbedoMap;

uniform vec3 _ambientLight;

vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / _renderingScreenSize;
}

void main()
{
	vec2 textCoord = CalcTexCoord();
	vec4 color = texture(_renderingDeferredAlbedoMap, textCoord);
	
	fragColor = color * vec4(_ambientLight, 1.0);
}
