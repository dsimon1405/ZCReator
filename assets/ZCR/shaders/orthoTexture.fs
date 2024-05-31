#version 460 core

in vec2 vTexCoords;

uniform sampler2D texColor;
uniform float unAlpha;

out vec4 FragColor;

void main()
{
    if (unAlpha == 0) discard;  //  check users alpha from uniform

    vec4 color = texture(texColor, vTexCoords);
    if (color.a == 0) discard;

    color.a *= unAlpha;   //  reduce font's alpha to users alpha

    FragColor = color;
}