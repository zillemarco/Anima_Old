#version 410

layout (location = 0) in vec3 _position;
layout (location = 2) in vec2 _textureCoord;

uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;

out vec2 frag_textureCoord;
out vec3 frag_worldPosition;

void main()
{
    gl_Position = viewProjectionMatrix * modelMatrix * vec4(_position, 1.0);
    frag_textureCoord = _textureCoord;
}
