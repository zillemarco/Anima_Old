#version 410 core

// PN patch data 
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

uniform mat4 viewProjectionMatrix;	// mvp uniform 
uniform mat4 modelMatrix;			// mvp uniform 
uniform float tessAlpha;			// controls the deformation   

layout(triangles, fractional_odd_spacing, ccw) in;   

in vec3 eval_normals[]; 
in vec2 eval_texCoords[]; 
in PnPatch eval_pnPatch[];   

out vec3 geom_worldPosition;
out vec3 geom_normal; 
out vec2 geom_textureCoord;   

#define b300    gl_in[0].gl_Position.xyz 
#define b030    gl_in[1].gl_Position.xyz 
#define b003    gl_in[2].gl_Position.xyz 
#define n200    eval_normals[0] 
#define n020    eval_normals[1] 
#define n002    eval_normals[2] 
#define uvw     gl_TessCoord   

void main() 
{  
	vec3 uvwSquared = uvw*uvw;  
	vec3 uvwCubed   = uvwSquared*uvw;    
	
	// extract control points  
	vec3 b210 = vec3(eval_pnPatch[0].b210, eval_pnPatch[1].b210, eval_pnPatch[2].b210);  
	vec3 b120 = vec3(eval_pnPatch[0].b120, eval_pnPatch[1].b120, eval_pnPatch[2].b120);  
	vec3 b021 = vec3(eval_pnPatch[0].b021, eval_pnPatch[1].b021, eval_pnPatch[2].b021);  
	vec3 b012 = vec3(eval_pnPatch[0].b012, eval_pnPatch[1].b012, eval_pnPatch[2].b012);  
	vec3 b102 = vec3(eval_pnPatch[0].b102, eval_pnPatch[1].b102, eval_pnPatch[2].b102);  
	vec3 b201 = vec3(eval_pnPatch[0].b201, eval_pnPatch[1].b201, eval_pnPatch[2].b201);  
	vec3 b111 = vec3(eval_pnPatch[0].b111, eval_pnPatch[1].b111, eval_pnPatch[2].b111);    
	
	// extract control normals  
	vec3 n110 = normalize(vec3(	eval_pnPatch[0].n110,                             
								eval_pnPatch[1].n110,                             
								eval_pnPatch[2].n110));  
								
	vec3 n011 = normalize(vec3( eval_pnPatch[0].n011,                             
								eval_pnPatch[1].n011,                             
								eval_pnPatch[2].n011));  
								
	vec3 n101 = normalize(vec3( eval_pnPatch[0].n101,                             
								eval_pnPatch[1].n101,                             
								eval_pnPatch[2].n101));    
								
	// compute texcoords  
	geom_textureCoord = gl_TessCoord[2]*eval_texCoords[0] + gl_TessCoord[0]*eval_texCoords[1] + gl_TessCoord[1]*eval_texCoords[2];    
	// normal  
	vec3 barNormal = gl_TessCoord[2]*eval_normals[0] + gl_TessCoord[0]*eval_normals[1] + gl_TessCoord[1]*eval_normals[2];  
	
	vec3 pnNormal  = n200*uvwSquared[2] + n020*uvwSquared[0] + n002*uvwSquared[1] + n110*uvw[2]*uvw[0] + n011*uvw[0]*uvw[1] + n101*uvw[2]*uvw[1];  
	geom_normal = tessAlpha*pnNormal + (1.0-tessAlpha)*barNormal;    
	geom_normal = (modelMatrix * vec4(geom_normal,1.0)).xyz;
	
	// compute interpolated pos  
	vec3 barPos = gl_TessCoord[2]*b300 + gl_TessCoord[0]*b030 + gl_TessCoord[1]*b003;    
	
	// save some computations  
	uvwSquared *= 3.0;    
	
	// compute PN position  
	vec3 pnPos  = b300*uvwCubed[2] + b030*uvwCubed[0] + b003*uvwCubed[1] + b210*uvwSquared[2]*uvw[0] + b120*uvwSquared[0]*uvw[2] + b201*uvwSquared[2]*uvw[1] + b021*uvwSquared[0]*uvw[1] + b102*uvwSquared[1]*uvw[2] + b012*uvwSquared[1]*uvw[0] + b111*6.0*uvw[0]*uvw[1]*uvw[2];    
	
	// final position and normal  
	vec3 finalPos = (1.0-tessAlpha)*barPos + tessAlpha*pnPos;  

	geom_worldPosition = (modelMatrix * vec4(finalPos,1.0)).xyz;

	gl_Position = viewProjectionMatrix * modelMatrix * vec4(finalPos,1.0); 
}