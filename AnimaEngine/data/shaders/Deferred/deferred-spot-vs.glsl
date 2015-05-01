#version 150 core

in vec3 _position;

uniform mat4 CAM_projectionViewMatrix;
uniform mat4 MOD_AModelMatrix;

out mat4 frag_inverseProjectionViewMatrix;

void main()
{          
    gl_Position = CAM_projectionViewMatrix * MOD_AModelMatrix * vec4(_position, 1.0);
    frag_inverseProjectionViewMatrix = inverse(CAM_projectionViewMatrix);
}