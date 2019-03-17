#version 410

in vec2 TextureCoord;

out vec4 FragColor;

uniform sampler2D GPosition;
uniform sampler2D GNormal;
uniform sampler2D BaseColor;

uniform vec4 LightPosition;
uniform vec3 LightColor;
uniform float LightIntensive;
uniform float F;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;

vec4 calculateLighting()
{
    vec4 position = texture(GPosition, TextureCoord);
    vec3 n = vec3(texture(GNormal, TextureCoord));
    vec3 v = vec3(-position);
    vec3 s = normalize(vec3(LightPosition - position));
    vec3 h = normalize(v + s);
    
    vec3 ambient = Ka ;
    vec3 diffuse = Kd * max(dot(s, n), 0.0);
    vec3 spec = Ks * pow(max(dot(n, h), 0.0), F);
    
    vec3 I = (ambient + diffuse + spec) * LightColor * LightIntensive;
    return vec4(I, 1.0);
}

void main()
{
    FragColor = texture(BaseColor, TextureCoord) * calculateLighting();
}
