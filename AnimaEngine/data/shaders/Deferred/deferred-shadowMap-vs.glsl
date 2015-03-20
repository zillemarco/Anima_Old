#version 150 core

in vec3 _position;

uniform mat4 DIL_ProjectionViewMatrix;
uniform mat4 MOD_modelMatrix;

void main()
{
    gl_Position = DIL_ProjectionViewMatrix * MOD_modelMatrix * vec4(_position, 1.0);
}
