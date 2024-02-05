#pragma once

#include <ZC/Video/OpenGL/Renderer/ZC_RendererSet.h>
#include <ZC/Video/OpenGL/Figure/ZC_Quad.h>

class ZCR_Triangle
{
public:
    ZC_DA<ZC_Quad> quads;
    ZC_DA<int> normals;
    ZC_Buffer vbo;
    ZC_RSNonTex rendSet;
    ZC_Uniforms* pUniforms = nullptr;

    ZCR_Triangle(ZC_DA<ZC_Quad>&& _quads, ZC_DA<unsigned char>& colors, ZC_DA<int>&& _normals, const ZC_Mat4<float>& model);

    ZCR_Triangle(ZCR_Triangle&& tr);

private:
    ZC_RSNonTex RendSet(ZC_DA<unsigned char>& colors);
    ZC_DA<unsigned char> GetElements(size_t quadsCount, size_t& rElementsCount, GLenum& rElementsType);
    
    template<typename TpHead>
    void FillElements(TpHead pIndex, size_t size);

    //  ДОБАВЛЕНИЕ ТЕКСТУРЫ В ZC_RendererSet
//  wall.jpg 512x512.png 200x200.png
    // std::vector<ZC_RendererSet::TexSet> texSets;
    // texSets.reserve(1);
    // auto texCreator = pShPInitSet->pTexSets->GetCreator();
    // for (auto i = texCreator.NextName(); i; i = texCreator.NextName())
    // {
    //     if (*i == ZC_TexSets::Name::color)
    //         texCreator.Add(ZC_Textures::LoadTexture2D("/home/dmitry/Загрузки/20x20.png"));
    // }
    // texSets.emplace_back(ZC_RendererSet::TexSet("", texCreator.GetVecTexture()));
};


template<typename TpHead>
void ZCR_Triangle::FillElements(TpHead pIndex, size_t size)
{
    for (size_t indicesI = 0, quadsI = 0; indicesI < size; ++quadsI, indicesI += 6)
    {
        pIndex[indicesI] = pIndex[indicesI + 3] = quadsI;
        pIndex[indicesI + 1] = pIndex[indicesI + 5] = ++quadsI;
        pIndex[indicesI + 2] = ++quadsI;
        pIndex[indicesI + 4] = ++quadsI;
    }
}