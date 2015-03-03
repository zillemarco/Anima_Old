#version 410

layout (location = 0) in vec3 _position;
layout (location = 1) in vec3 _normal;
layout (location = 2) in vec2 _textureCoord;
layout (location = 3) in vec3 _tangent;
layout (location = 4) in vec3 _bitangent;
//layout (location = 3) in vec3 _color;

uniform mat4 CAM_projectionViewMatrix;
uniform mat4 MOD_modelMatrix;

out vec2 frag_textureCoord;
out vec3 frag_normal;
out vec3 frag_tangent;
out vec3 frag_bitangent;
out vec3 frag_worldPosition;

void main()
{
    gl_Position = CAM_projectionViewMatrix * MOD_modelMatrix * vec4(_position, 1.0);
    frag_textureCoord = _textureCoord;
    frag_normal = (MOD_modelMatrix * vec4(_normal, 0.0)).xyz;
    frag_tangent = (MOD_modelMatrix * vec4(_tangent, 0.0)).xyz;
    frag_bitangent = (MOD_modelMatrix * vec4(_bitangent, 0.0)).xyz;
	frag_worldPosition = (MOD_modelMatrix * vec4(_position, 1.0)).xyz;
}
