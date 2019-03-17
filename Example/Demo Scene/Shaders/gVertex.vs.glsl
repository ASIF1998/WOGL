#version 410

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexTextureCoord;
layout(location = 3) in vec3 VertexTangent;

uniform mat4 MV;
uniform mat4 MVP;
uniform mat3 NormalMatrix;

uniform float Scale;

out vec3 Position;
out vec2 TextureCoord;
out mat3 TBN;

mat3 scale()
{
    return mat3 (
        Scale, 0.0, 0.0,
        0.0, Scale, 0.0,
        0.0, 0.0, Scale
    );
}

void main()
{
    vec3 normal = normalize(NormalMatrix * VertexNormal);
    vec3 tangent = normalize(NormalMatrix * VertexTangent);
    vec3 btangent = normalize(cross(normal, tangent));
    
    Position = vec3(MV * vec4(scale() * VertexPosition, 1.0));
    TextureCoord = VertexTextureCoord;
    TBN = mat3(tangent, btangent, normal);
    gl_Position = MVP * vec4(scale() * VertexPosition, 1.0);
}
