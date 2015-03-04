#version 150 core

in vec3 _position;
in vec3 _normal;
in vec2 _textureCoord;
in vec3 _tangent;
in vec3 _bitangent;

uniform mat4 CAM_projectionViewMatrix;
uniform mat4 MOD_modelMatrix;

out vec2 frag_textureCoord;
out vec3 frag_normal;
out vec3 frag_tangent;
out vec3 frag_bitangent;
out vec3 frag_worldPosition;
out mat3 frag_TBNMatrix;

void main()
{
    gl_Position = CAM_projectionViewMatrix * MOD_modelMatrix * vec4(_position, 1.0);
    
	frag_normal = (MOD_modelMatrix * vec4(_normal, 0.0)).xyz;
    frag_tangent = (MOD_modelMatrix * vec4(_tangent, 0.0)).xyz;
    frag_bitangent = (MOD_modelMatrix * vec4(_bitangent, 0.0)).xyz;
	frag_worldPosition = (MOD_modelMatrix * vec4(_position, 1.0)).xyz;
    frag_textureCoord = _textureCoord;
	frag_TBNMatrix = mat3(frag_tangent, frag_bitangent, frag_normal);
}
