#version 410

layout (location = 0) in vec3 _position;
layout (location = 1) in vec3 _normal;
layout (location = 2) in vec2 _textureCoord;
//layout (location = 3) in vec3 _color;

//uniform mat4 viewProjectionMatrix;
//uniform mat4 modelMatrix;

out vec2 ctrl_textureCoord;
out vec3 ctrl_normal;
//out vec3 ctrl_worldPosition;

void main()
{
    //gl_Position = viewProjectionMatrix * modelMatrix * vec4(_position, 1.0);
    //ctrl_normal = (modelMatrix * vec4(_normal, 0.0)).xyz;
	//ctrl_worldPosition = (modelMatrix * vec4(_position, 1.0)).xyz;

    ctrl_textureCoord = _textureCoord;
    ctrl_normal = _normal;
	gl_Position = vec4(_position, 1.0);
}
