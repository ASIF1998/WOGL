#version 410

in vec2 TextureCoord;

out vec4 FragColor;

uniform sampler2D Texture;

void main()
{
    FragColor = texture(Texture, TextureCoord);
}
