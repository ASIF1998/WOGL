#version 410

out vec4 FragColor;

in vec2 TexCoord;
in vec4 Position;
in vec4 PositionInLightSpace;
in vec3 Normal;

uniform sampler2D ModelTexture;
uniform sampler2D NormalMap;
uniform sampler2D ShadowMap;

uniform mat3 NormalMatrix;

uniform vec4 LightPosition;

uniform vec3 LightColor;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;

uniform float F;
uniform float LightIntensive;

float shadowCalculation()
{
    /// Так позиция источника света возвращается не в gl_Positiion^ то необходимо
    /// его нармализовать путём деления первых 3-х компанентов ветора на 4-ый.
    vec3 projCoords = PositionInLightSpace.xyz / PositionInLightSpace.w;
    
    /// Приводим коорлинаты из диапозона [-1, 1] к диопозону [0, 1]
    projCoords = projCoords * vec3(0.5) + vec3(0.5);
    
    float colsestDepth = texture(ShadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z - 0.05;
    
    return currentDepth > colsestDepth ? 0.0 : 1.0;
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

    return (ambinent + shadowCalculation () * (diffuse + specular)) * pow(LightIntensive, F);
}

void main()
{
    FragColor =  texture(ModelTexture, TexCoord) * vec4(calculateLighting(), 1.0);
}

