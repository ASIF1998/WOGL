#version 410

out vec4 FragColor;

in vec3 Position;
in vec2 TextureCoord;
in mat3 TBN;

uniform sampler2D BaseColorTexture;
uniform sampler2D NormalMap;

layout(location = 0) out vec3 GPosition;
layout(location = 1) out vec3 GNormal;
layout(location = 2) out vec3 BaseColor;

void main()
{
    GPosition = Position;
    GNormal = TBN * texture(NormalMap, TextureCoord).xyz;
    BaseColor = texture(BaseColorTexture, TextureCoord).rgb;
}
