#version 330                                                                        

layout (location = 0) in vec3 _position; 

uniform mat4 CAM_projectionViewMatrix;
uniform mat4 MOD_modelMatrix;

void main()
{          
    gl_Position = CAM_projectionViewMatrix * MOD_modelMatrix * vec4(_position, 1.0);
}