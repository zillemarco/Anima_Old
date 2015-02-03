#version 410

in vec2 frag_textureCoord;
in vec3 frag_worldPosition;

out vec4 fragColor;

uniform sampler2D materialDiffuseTexture;
uniform vec4 materialColor;
uniform vec3 ambientLight;

void main()
{
	vec4 color = materialColor;
    vec4 textureColor = texture(materialDiffuseTexture, frag_textureCoord.xy);
	
	if(textureColor != vec4(0.0, 0.0, 0.0, 1.0))
		color *= textureColor;
		
	fragColor = color * vec4(ambientLight, 1.0);
}
