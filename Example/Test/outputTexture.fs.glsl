#version 410

in vec2 TextureCoord;

out vec4 FragColor;

uniform sampler2D Texture;

void main()
{
    FragColor = texture(Texture, TextureCoord);
    // FragColor = vec4(1.0, 1.0, 0.0, 0.0);
}