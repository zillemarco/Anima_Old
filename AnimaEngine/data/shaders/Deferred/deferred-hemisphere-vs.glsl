#version 410 core

in vec3 _position;

uniform mat4 MOD_AModelMatrix;
void main()
{          
    gl_Position = MOD_AModelMatrix * vec4(_position, 1.0);
}