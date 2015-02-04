#version 410

layout(triangles) in;
layout(triangle_strip) out;
layout(max_vertices=3) out;

in vec3 geom_worldPosition[3];
in vec2 geom_textureCoord[3];
in vec3 geom_normal[3];

out vec2 frag_textureCoord;
out vec3 frag_normal;
out vec3 frag_worldPosition;
noperspective out vec3 frag_dist;

uniform vec2 windowSize;

void main(void)
{
	vec2 p0 = windowSize * gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w;
	vec2 p1 = windowSize * gl_in[1].gl_Position.xy / gl_in[1].gl_Position.w;
	vec2 p2 = windowSize * gl_in[2].gl_Position.xy / gl_in[2].gl_Position.w;
	vec2 v0 = p2 - p1;
	vec2 v1 = p2 - p0;
	vec2 v2 = p1 - p0;
	float area = abs(v1.x * v2.y - v1.y * v2.x);

	frag_dist = vec3(area / length(v0), 0.0, 0.0);
	frag_worldPosition = geom_worldPosition[0];
	frag_textureCoord = geom_textureCoord[0];
	frag_normal = geom_normal[0];
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	
	frag_dist = vec3(0.0, area / length(v1), 0.0);
	frag_worldPosition = geom_worldPosition[1];
	frag_textureCoord = geom_textureCoord[1];
	frag_normal = geom_normal[1];
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();
	
	frag_dist = vec3(0.0, 0.0, area / length(v2));
	frag_worldPosition = geom_worldPosition[2];
	frag_textureCoord = geom_textureCoord[2];
	frag_normal = geom_normal[2];
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();
	
	EndPrimitive();
}
