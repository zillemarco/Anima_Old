#version 410 core

struct PnPatch 
{
    float b210;
    float b120;
    float b021;
    float b012;
    float b102;
    float b201;
    float b111;
    float n110;
    float n011;
    float n101;
};

uniform float tessellationLevel;
uniform vec3 ctrl_eyePosition;
uniform mat4 ctrl_modelMatrix;			// mvp uniform 

layout(vertices=3) out;
in vec3 ctrl_normal[];
in vec2 ctrl_textureCoord[];

out vec3 eval_normals[3];
out vec2 eval_texCoords[3];
out PnPatch eval_pnPatch[3];

float wij(int i, int j) 
{  
    return dot(gl_in[j].gl_Position.xyz - gl_in[i].gl_Position.xyz, ctrl_normal[i]); 
}   

float vij(int i, int j) 
{  
    vec3 Pj_minus_Pi = gl_in[j].gl_Position.xyz - gl_in[i].gl_Position.xyz;  
    vec3 Ni_plus_Nj = ctrl_normal[i]+ctrl_normal[j];  
    return 2.0*dot(Pj_minus_Pi, Ni_plus_Nj) / dot(Pj_minus_Pi, Pj_minus_Pi); 
}   

float GetTessLevel(float Distance0, float Distance1)                                            
{                                                                                               
    float AvgDistance = (Distance0 + Distance1) / 2.0;                                          
                                                                                                
    if (AvgDistance <= 10.0) {                                                                   
        return 10.0;                                                                            
    }                                                                                           
    else if (AvgDistance <= 20.0) {                                                              
        return 7.0;                                                                             
    }                                                                                           
    else {                                                                                      
        return 3.0;                                                                             
    }                                                                                           
}  

void main() 
{  
    // get data  
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;  
    eval_normals[gl_InvocationID]       = ctrl_normal[gl_InvocationID];  
    eval_texCoords[gl_InvocationID]     = ctrl_textureCoord[gl_InvocationID];    
    
    // set base   
    float P0 = gl_in[0].gl_Position[gl_InvocationID];  
    float P1 = gl_in[1].gl_Position[gl_InvocationID];  
    float P2 = gl_in[2].gl_Position[gl_InvocationID];  
    float N0 = ctrl_normal[0][gl_InvocationID];  
    float N1 = ctrl_normal[1][gl_InvocationID];  
    float N2 = ctrl_normal[2][gl_InvocationID];    

    // compute control points  
    eval_pnPatch[gl_InvocationID].b210 = (2.0*P0 + P1 - wij(0,1)*N0)/3.0;  
    eval_pnPatch[gl_InvocationID].b120 = (2.0*P1 + P0 - wij(1,0)*N1)/3.0;  
    eval_pnPatch[gl_InvocationID].b021 = (2.0*P1 + P2 - wij(1,2)*N1)/3.0;  
    eval_pnPatch[gl_InvocationID].b012 = (2.0*P2 + P1 - wij(2,1)*N2)/3.0;  
    eval_pnPatch[gl_InvocationID].b102 = (2.0*P2 + P0 - wij(2,0)*N2)/3.0;  
    eval_pnPatch[gl_InvocationID].b201 = (2.0*P0 + P2 - wij(0,2)*N0)/3.0;  

    float E = ( eval_pnPatch[gl_InvocationID].b210 +
                eval_pnPatch[gl_InvocationID].b120 + 
                eval_pnPatch[gl_InvocationID].b021 + 
                eval_pnPatch[gl_InvocationID].b012 + 
                eval_pnPatch[gl_InvocationID].b102 + 
                eval_pnPatch[gl_InvocationID].b201 ) / 6.0;  

    float V = (P0 + P1 + P2) / 3.0;  

    eval_pnPatch[gl_InvocationID].b111 = E + (E - V)*0.5;  
    eval_pnPatch[gl_InvocationID].n110 = N0+N1-vij(0,1)*(P1-P0);  
    eval_pnPatch[gl_InvocationID].n011 = N1+N2-vij(1,2)*(P2-P1);  
    eval_pnPatch[gl_InvocationID].n101 = N2+N0-vij(2,0)*(P0-P2); 

    float EyeToVertexDistance0 = distance(ctrl_eyePosition, (ctrl_modelMatrix * gl_in[0].gl_Position).xyz);
    float EyeToVertexDistance1 = distance(ctrl_eyePosition, (ctrl_modelMatrix * gl_in[1].gl_Position).xyz);
    float EyeToVertexDistance2 = distance(ctrl_eyePosition, (ctrl_modelMatrix * gl_in[2].gl_Position).xyz);
    
    // set tess levels  
    gl_TessLevelOuter[0] = GetTessLevel(EyeToVertexDistance1, EyeToVertexDistance2);
    gl_TessLevelOuter[1] = GetTessLevel(EyeToVertexDistance2, EyeToVertexDistance0);
    gl_TessLevelOuter[2] = GetTessLevel(EyeToVertexDistance0, EyeToVertexDistance1);
    //gl_TessLevelOuter[3] = tessLOD;

    gl_TessLevelInner[0] = gl_TessLevelOuter[2];
    //gl_TessLevelInner[1] = tessLOD; 
}