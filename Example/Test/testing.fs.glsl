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

/**
 * Вычисление света по Фонгу с помощью вектора полупути
*/
vec3 calculateLighting()
{
    vec3 n = normalize(NormalMatrix * Normal);
    vec3 s = normalize(LightPosition.xyz - Position.xyz);
    vec3 v = -Position.xyz;
    vec3 h = normalize(v + s);

    vec3 ambinent = LightColor * Ka;
    vec3 diffuse = LightColor * Kd * max(dot(n, s), 0.0);
    vec3 specular = LightColor * Ks * max(dot(h, v), 0.0);

    return (ambinent + diffuse + specular) * pow(LightIntensive, F);
}

void main()
{
    FragColor =  texture(modelTexture, TexCoord) * vec4(calculateLighting(), 1.0);
}

