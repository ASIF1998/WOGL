#version 410

in vec2 TextureCoords;

uniform sampler2D Texture;

out vec4 FragColor;

void main()
{
    vec2 texCoord = vec2(TextureCoords.x, 1.0 - TextureCoords.y);
    FragColor = texture(Texture, texCoord);
}
