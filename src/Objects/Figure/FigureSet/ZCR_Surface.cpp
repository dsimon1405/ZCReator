#include "ZCR_Surface.h"

#include <ZC/Tools/Math/ZC_Math.h>

ZCR_Surface::ZCR_Surface()
    : spRendererSetsSucafe(MakeSurfaceRendererSet()),
    rsControllerSurface(MakeSurfaceRSController())
{}

ZC_sptr<ZC_RendererSet> ZCR_Surface::MakeSurfaceRendererSet()
{
    size_t elementsCount = 0;
    GLenum elementsType = 0;
    ZC_DA<uchar> elements = GetTriangleElements(elementsCount, elementsType);

    ZC_Buffer ebo(GL_ELEMENT_ARRAY_BUFFER);
    ebo.BufferData(elements.size, elements.Begin(), GL_STATIC_DRAW);

    auto upDraw = ZC_uptrMakeFromChild<ZC_GLDraw, ZC_DrawElements>(GL_TRIANGLES, elementsCount, elementsType, 0);

    typename ZC_ShProgs::ShPInitSet* pShPIS = ZC_ShProgs::Get(ZC_ShProgs::Name::ZCR_ColorFigure);

    ZC_VAO vao;
    vao.Config(pShPIS->vaoConfigData, *vbo, &ebo, 0, (spQuads ? (spQuads->size * 4) : 0) + (spTriangles ? (spTriangles->size * 3) : 0));   //  vertices count in vbo (in one quad 4, in one triangle 3)

    std::forward_list<ZC_Buffer> buffers;
    buffers.emplace_front(std::move(ebo));

    return ZC_RendererSet::CreateShptr(pShPIS, std::move(vao), std::move(upDraw), std::move(buffers));
    // switch (element)
    // {
    // case GLElement::Triangle: return { new ZC_RSNonTex(pShPIS, std::move(vao), std::move(upDraw), std::move(buffers)) };
    // case GLElement::Point: return { new ZC_RSNonTex(pShPIS, std::move(vao), std::move(upDraw), std::move(buffers)) };
    // case GLElement::Line: return { new ZC_RSNonTex(pShPIS, std::move(vao), std::move(upDraw), std::move(buffers)) };
    // }
}

ZC_RSController ZCR_Surface::MakeSurfaceRSController()
{
    // std::forward_list<ZC_uptr<ZC_RSPersonalData>>{ZC_RSPDStencilBorder::Make({ 1.05f, ZC_PackColorUCharToUInt(255, 90, 0) })};
    // ZC_RSPDStencilBorder::Make({ 1.05f, ZC_PackColorUCharToUInt(255, 90, 0) });

    std::forward_list<ZC_uptr<ZC_RSPersonalData>> persDatas;
    persDatas.emplace_front(ZC_RSPDStencilBorder::Make({ 1.05f, ZC_PackColorUCharToUInt(255, 90, 0) }));    //  stencil border color {r = 255, g = 90, b = 0}
    return spRendererSetsSucafe->MakeZC_RSController(-1, std::move(persDatas));
}

ZC_DA<uchar> ZCR_Surface::GetTriangleElements(size_t& rElementsCount, GLenum& rElementsType)
{
    size_t quadsElementsCount = spQuads ? (spQuads->size * 6) : 0,     //  6 elements in ebo on one quad
        trianglesElementsCount = spTriangles ? (spTriangles->size * 3) : 0;     //  3 elements in ebo on one triangle
    rElementsCount = quadsElementsCount + trianglesElementsCount;  
    size_t verticesInVBO = (spQuads ? (spQuads->size * 4) : 0) + trianglesElementsCount,     //  4 vertices in vbo on one quad
        storingTypeSize = 0;
    ZC_Buffer::GetElementsData(verticesInVBO - 1, storingTypeSize, rElementsType);
    ZC_DA<uchar> elements(storingTypeSize * rElementsCount);
    switch (storingTypeSize)
    {
    case 1: FillTriangleElements(elements.pHead, elements.size, quadsElementsCount); break;
    case 2: FillTriangleElements(reinterpret_cast<ushort*>(elements.pHead), elements.size / 2, quadsElementsCount); break;
    case 4: FillTriangleElements(reinterpret_cast<uint*>(elements.pHead), elements.size / 4, quadsElementsCount); break;
    }
    return elements;
}

void ZCR_Surface::SwitchRSandDSTriangle(ZC_RendererLevel lvl)
{
    rsControllerSurface.SwitchToLvl(lvl);
}