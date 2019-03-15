#version 410

out vec4 FragColor;

in vec2 TexCoord;
in vec4 Position;
in vec4 PositionInLightSpace;
in vec3 Normal;

uniform sampler2D ModelTexture;
uniform sampler2D NormalMap;
uniform sampler2DShadow ShadowMap;

uniform mat3 NormalMatrix;
uniform mat4 LightSpaceMatrix;

uniform vec4 LightPosition;
uniform vec3 LightColor;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;

uniform float F;
uniform float LightIntensive;

float shadowCalculation()
{
    /// Приводим коорлинаты из диапозона [-1, 1] к диопозону [0, 1]
    vec4 projCoords = PositionInLightSpace * vec4(0.5) + vec4(0.5);
    
    float sum = textureProjOffset(ShadowMap, vec4(projCoords.xyz, 1.0), ivec2(1, 1));
    sum += textureProjOffset(ShadowMap, vec4(projCoords.xyz, 1.0), ivec2(-1, 1));
    sum += textureProjOffset(ShadowMap, vec4(projCoords.xyz, 1.0), ivec2(1, -1));
    sum += textureProjOffset(ShadowMap, vec4(projCoords.xyz, 1.0), ivec2(-1, -1));
    
    return sum * 0.25;
}

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

    return (ambinent + shadowCalculation() * (diffuse + specular)) * pow(LightIntensive, F);
}

void main()
{
    FragColor =  texture(ModelTexture, TexCoord) * vec4(calculateLighting(), 1.0);
}

