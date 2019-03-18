#version 410

// Количество выборок ядра.
#define KERNEL_SIZE 64

// Радиус ядра выборки выборки эффекта SSAO.
#define RADIUS 0.5

#define RANGE_CHECK(fragmentPosition, sampleDepth) \
        smoothstep(0.0, 1.0, RADIUS / abs(fragmentPosition.z - sampleDepth))

in vec2 TextureCoord;

out float FragColor;

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

    vec3 s;
    vec4 offset;
    float sDepth, occlusion = 0.0;

    for (int i = 0; i < KERNEL_SIZE; i++) {
        s = tbn * Samples[i];
        s = positon + s * RADIUS;

        // Переходим из видового пространства в клиповое.
        offset = ProjectionMatrix * vec4(s, 1.0);
        offset.xyz /= offset.w;

        // Преобразуем координаты из диапазона [-1, 1] в диапазон [0, 1].
        offset.xyz = offset.xyz * 0.5 + 0.5;

        // Осуществляем выборку.
        sDepth = texture(GPosition, offset.xy).z;

        occlusion += (sDepth >= s.z + 0.025 ? 1.0 : 0.0) * RANGE_CHECK(positon, sDepth);
    }

    FragColor = 1.0 - occlusion / KERNEL_SIZE;
}
