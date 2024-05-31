#version 460 core

in vec2 vTexCoords;

uniform sampler2D texColor;

out vec4 FragColor;

void main()
{
    vec4 texColor = texture(texColor, vTexCoords);
    // if (texColor.a == 0) discard;
    FragColor = texColor;
}