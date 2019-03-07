#version 410

out vec4 FragColor;

in vec2 TexCoord;
in vec4 Position;
in vec3 Normal;

uniform sampler2D modelTexture;
uniform sampler2D normalMap;

uniform mat3 NormalMatrix;

uniform vec4 LightPosition;

uniform vec3 LightColor;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;

uniform float F;
uniform float LightIntensive;

void main()
{
    FragColor = texture(modelTexture, TexCoord);
}

