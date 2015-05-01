#version 150 core

in vec3 _position;

uniform mat4 CAM_projectionViewMatrix;
uniform mat4 MOD_AModelMatrix;

void main()
{          
    gl_Position = MOD_AModelMatrix * vec4(_position, 1.0);
}