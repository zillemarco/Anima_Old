#version 410

layout (location = 0) in vec3 _position;
layout (location = 1) in vec3 _normal;
layout (location = 2) in vec2 _textureCoord;

uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;

out vec2 frag_textureCoord;
out vec3 frag_normal;
out vec3 frag_worldPosition;

void main()
{
    gl_Position = viewProjectionMatrix * modelMatrix * vec4(_position, 1.0);
    frag_textureCoord = _textureCoord;
    frag_normal = (modelMatrix * vec4(_normal, 0.0)).xyz;
	frag_worldPosition = (modelMatrix * vec4(_position, 1.0)).xyz;
}
