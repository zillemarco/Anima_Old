#version 150 core

in vec3 _position;
in vec3 _normal;
in vec2 _textureCoord;
in vec3 _tangent;
in vec3 _bitangent;
in vec4 _boneWeights;
in vec4 _boneIDs;

uniform mat4 CAM_ProjectionViewMatrix;
uniform mat4 MOD_AModelMatrix;
uniform mat4 MOD_ANormalMatrix;
uniform mat4 MOD_BonesTransformations[100];

out vec2 frag_textureCoord;
out vec3 frag_normal;
out vec3 frag_worldPosition;
out mat3 frag_TBNMatrix;

void main()
{
    mat4 BoneTransform = MOD_BonesTransformations[int(_boneIDs[0])] * _boneWeights[0];
    BoneTransform     += MOD_BonesTransformations[int(_boneIDs[1])] * _boneWeights[1];
    BoneTransform     += MOD_BonesTransformations[int(_boneIDs[2])] * _boneWeights[2];
    BoneTransform     += MOD_BonesTransformations[int(_boneIDs[3])] * _boneWeights[3];
	
	vec4 boneSpacePosition = BoneTransform * vec4(_position, 1.0);
	vec4 boneSpaceNormal = BoneTransform * vec4(_normal, 0.0);
	vec4 boneSpaceTangent = BoneTransform * vec4(_tangent, 0.0);
	vec4 boneSpaceBitangent = BoneTransform * vec4(_bitangent, 0.0);

    gl_Position = CAM_ProjectionViewMatrix *  MOD_AModelMatrix * boneSpacePosition;
    
	frag_normal = normalize((MOD_ANormalMatrix * boneSpaceNormal).xyz);
    vec3 tangent = normalize((MOD_ANormalMatrix * boneSpaceTangent).xyz);
    vec3 bitangent = normalize((MOD_ANormalMatrix * boneSpaceBitangent).xyz);
	frag_worldPosition = (MOD_AModelMatrix * boneSpacePosition).xyz;
    frag_textureCoord = _textureCoord;
	frag_TBNMatrix = mat3(tangent, bitangent, frag_normal);
}
