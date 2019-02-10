#version 410

in vec3 Color;
in vec2 TextureCoords;

uniform sampler2D Texture;

out vec4 FragColor;

void main()
{
//    FragColor = vec4(Color, 1.0);
//    FragColor = vec4(TextureCoords.x, TextureCoords.y, 0.0, 1.0);
    vec2 texCoord = vec2(TextureCoords.x, 1.0 - TextureCoords.y);
    FragColor = texture(Texture, texCoord);
}
