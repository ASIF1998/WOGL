#version 410

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTextureCoord;
layout (location = 3) in vec3 VertexTangent;

out vec2 TexCoord;
out vec4 Position;
out vec4 PositionInLightSpace;
out vec3 Normal;
out mat3 TBN;

uniform mat4 M;
uniform mat4 MV;
uniform mat4 MVP;
uniform mat4 LightSpaceMatrix;
uniform mat3 NormalMatrix;

uniform vec4 LightPosition;

uniform float ScaleX;
uniform float ScaleY;
uniform float ScaleZ;

mat3 scaling()
{
    return mat3 (
        ScaleX, 0.0, 0.0,
        0.0, ScaleY, 0.0,
        0.0, 0.0, ScaleZ
    );
}

void main()
{
    vec3 n = normalize(NormalMatrix * VertexNormal);
    vec3 t = normalize(NormalMatrix * VertexTangent);
    vec3 b = normalize(cross(n, t));
    
    TexCoord = VertexTextureCoord;
    Position = normalize(MV * vec4(VertexPosition, 1.0));
    Normal = VertexNormal;
    PositionInLightSpace = LightSpaceMatrix * vec4(VertexPosition, 1.0);
    TBN = mat3(t, b, n);

    gl_Position = MVP * vec4(scaling() * VertexPosition, 1.0);
}

