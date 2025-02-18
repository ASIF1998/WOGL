#version 410

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexTextureCoords;

out vec2 TextureCoords;

void main()
{
    TextureCoords = VertexTextureCoords;
    gl_Position = vec4(VertexPosition, 1.0);
}
