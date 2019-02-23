#version 410

out vec4 FragColor;

in vec2 TexCoords;
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
//    vec3 n = max(texture(normalMap, TexCoords).xyz * vec3(2.0) - vec3(1.0), 0.0);
//    n = normalize(NormalMatrix * n);
//
//    vec3 s = normalize(vec3(LightPosition - Position));
//    vec3 v = -s;
//    vec3 r = reflect(v, n);
//
//    float Dot = max(dot(n, s), 0.0);
//
//    vec3 Ia = LightColor * Ka;
//    vec3 Id = LightColor * Kd * Dot;
//    vec3 Is = vec3(0.0);
//
//    if (Dot > 0.0) {
//        Is = LightColor * Ks * pow(max(dot(v, r), 0.0), F);
//    }
//
//    vec3 I = Ia + Id + Is;
//    I *= LightIntensive;
    
    FragColor = texture(modelTexture, TexCoords);
    
//    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
//    FragColor = vec4(Normal, 1.0);
}

