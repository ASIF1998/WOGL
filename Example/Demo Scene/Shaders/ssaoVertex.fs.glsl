#version 410

in vec2 TextureCoord;

out vec4 FragColor;

uniform vec3 Samples[64];
uniform mat4 ProjectionMatrix;

uniform sampler2D GPosition;
uniform sampler2D GNormal;
uniform sampler2D TextureNoise;

vec2 NoiseScale = vec2(250.0, 250.0f);

void main()
{
    vec3 positon = texture(GPosition, TextureCoord).xyz;
    vec3 normal = texture(GNormal, TextureCoord).xyz;
    vec3 randomVector = texture(TextureNoise, TextureCoord * NoiseScale).xyz;
    
    vec3 tangent = normalize(randomVector - normal * dot(randomVector, normal));
    vec3 btangent = cross(normal, tangent);
    mat3 tbn = mat3(tangent, btangent, normal);
    
    FragColor = texture(GNormal, TextureCoord);
}
