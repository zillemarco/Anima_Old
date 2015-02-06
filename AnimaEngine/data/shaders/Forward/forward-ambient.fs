#version 410

in vec2 frag_textureCoord;
in vec3 frag_worldPosition;

out vec4 fragColor;

uniform sampler2D _materialDiffuseTexture;
uniform vec4 _materialDiffuseColor;
uniform vec3 _ambientLight;

void main()
{
	vec4 color = _materialDiffuseColor;
    vec4 textureColor = texture(_materialDiffuseTexture, frag_textureCoord.xy);
	
	if(textureColor != vec4(0.0, 0.0, 0.0, 1.0))
		color *= textureColor;
		
	fragColor = color * vec4(_ambientLight, 1.0);
}
