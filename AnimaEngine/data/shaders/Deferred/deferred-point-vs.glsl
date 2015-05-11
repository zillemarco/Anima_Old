#version 410 core

in vec3 _position;

uniform mat4 CAM_InverseProjectionViewMatrix;
uniform mat4 CAM_ProjectionViewMatrix;
uniform mat4 MOD_AModelMatrix;

out mat4 frag_inverseProjectionViewMatrix;

void main()
{          
    gl_Position = CAM_ProjectionViewMatrix * MOD_AModelMatrix * vec4(_position, 1.0);
    frag_inverseProjectionViewMatrix = CAM_InverseProjectionViewMatrix;
}