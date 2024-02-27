#pragma once

#include <ZC/Video/OpenGL/Texture/ZC_Texture.h>
#include <ZC/Tools/Math/ZC_Limits.h>
#include <imgui.h>

struct ZCR_IconTexture
{
    static inline ZC_Texture iconTexture;

    struct TexCoords
    {
        ImVec2 tl, br;
    };

private:
    static constexpr float texWidth = 720.f,
        texHeight =  150.f;

    static TexCoords CalculateTexCoords(int line, int column)
    {
        switch (line)
        {
        case 1:
        {
            static constexpr float startY = 0.f / texHeight,
                firstLineHeight = 150.f,
                endY = firstLineHeight / texHeight,
                stepX = 180.f;
            return { { ((column - 1) * stepX) / texWidth, startY }, { (column * stepX) / texWidth ,endY } };
        }
        default: return {};
        }
    }

public:
    //  first line
    static inline const TexCoords tcCreateCube = CalculateTexCoords(1, 1);
    static inline const TexCoords tcCreateCone = CalculateTexCoords(1, 2);
    static inline const TexCoords tcCreateCylinder = CalculateTexCoords(1, 3);
    static inline const TexCoords tcCreateSphere = CalculateTexCoords(1, 4);
};