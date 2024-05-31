#version 460 core

in vec2 vTexCoords;

uniform sampler2D texColor;
uniform uint unColor;
uniform float unAlpha = 1;  //  uses only in ZC_TextWindow

out vec4 FragColor;

void main()
{
    if (unAlpha == 0) discard;  //  check users alpha from uniform (sets only in ZC_TextWindow)

    float alpha = texture(texColor, vTexCoords).r;
    if (alpha < 0.2) discard;   //  optimal variant for Ortho and perspective Blending

    alpha *= unAlpha;   //  reduce font's alpha to users alpha (only for ZC_TextWindow)

    float r = (unColor >> 20) / 255.f;
    float g = (unColor >> 10 & uint(1023)) / 255.f;
    float b = (unColor & uint(1023)) / 255.f;

    FragColor = vec4(r, g, b, alpha);
}