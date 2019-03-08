#version 410

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTextureCoord;

out vec2 TexCoord;
out vec4 Position;
out vec3 Normal;

uniform mat4 MV;
uniform mat4 MVP;

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
    TexCoord = VertexTextureCoord;
    Position = MV * vec4(VertexPosition, 1.0);
    Normal = VertexNormal;

    gl_Position = MVP * vec4(scaling() * VertexPosition, 1.0);
}
