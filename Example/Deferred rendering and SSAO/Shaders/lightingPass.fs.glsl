#version 410

in vec2 TextureCoord;

out vec4 FragColor;

uniform sampler2D GPosition;
uniform sampler2D GNormal;
uniform sampler2D BaseColor;
uniform sampler2D SSAO;

uniform vec4 LightPosition;
uniform vec3 LightColor;
uniform float LightIntensive;
uniform float F;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;

/**
 * Функция предназначенная для размытия фонового затенения.
 */
float blurSSAO()
{
    vec2 texelSize = 1.0 / vec2(textureSize(SSAO, 0));
    float res = 0.0;
    
    for (int x = -2; x < 2; x++) {
        for (int y = -2; y < 2; y++) {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            res += texture(SSAO, TextureCoord + offset).r;
        }
    }
    
    return res / 16.0;
}

void main()
{
    vec4 position = texture(GPosition, TextureCoord);
    vec3 n = vec3(texture(GNormal, TextureCoord));
    vec3 v = vec3(-position);
    vec3 s = normalize(vec3(LightPosition - position));
    vec3 h = normalize(v + s);
    
    vec3 ambient = Ka * blurSSAO();
    vec3 diffuse = Kd * max(dot(s, n), 0.0);
    vec3 spec = Ks * pow(max(dot(n, h), 0.0), F);
    
    vec3 I = (ambient + diffuse + spec) * LightColor * LightIntensive;
    
    FragColor = texture(BaseColor, TextureCoord) * vec4(I, 1.0);
}
