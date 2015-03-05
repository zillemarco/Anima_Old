#version 150 core

in vec3 _position;
out mat4 frag_inverseProjectionViewMatrix;

uniform mat4 CAM_projectionViewMatrix;
uniform mat4 MOD_modelMatrix;

void main()
{          
    gl_Position = CAM_projectionViewMatrix * MOD_modelMatrix * vec4(_position, 1.0);
}