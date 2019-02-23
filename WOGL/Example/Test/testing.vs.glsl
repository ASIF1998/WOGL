#version 410

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec2 TexCoords;
out vec4 Position;
out vec3 Normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
    TexCoords = texCoord;
    Position = ViewMatrix * ModelMatrix * vec4(pos, 1.0);
    Normal = normal;
    
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(pos, 1.0);
//    gl_Position = vec4(pos, 1.0);
}

