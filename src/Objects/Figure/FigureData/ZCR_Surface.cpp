#include "ZCR_Surface.h"

#include <ZC/Tools/Math/ZC_Math.h>
#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <Objects/Scene/ImGui/ZCR_IGWBM_Mode.h>
#include <Renderer/ZCR_RenderLevel.h>

ZCR_Surface::ZCR_Surface()
    : dsSurface(CreateSurfaceDrawerSet()),
    dscSurface(MakeSurfaceRSController())
{
    dscSurface.SetUniformsData(ZC_UN_unAlpha, &alpha);
    dscSurface.SetUniformsData(ZCR_UN_unUseLight, &useLight);
}

// ZCR_Surface::ZCR_Surface(ZCR_Surface&& s)
//     : dsSurface(std::move(s.dsSurface)),
//     dsConSurface(MakeSurfaceRSController())
// {
//     dsConSurface.SwitchToDrawLvl(ZC_RL_Default, s.dsConSurface.GetDrawingLevel(ZC_RL_Default));   //  switch new controller to acoding level of previous controller
// }

void ZCR_Surface::ChangeSceneModeTriangle(ZCR_SceneMode sceneMode)
{
    if (alpha == 0.f) dscSurface.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_None);
    else if (alpha < 1.f)
    {
        if (sceneMode == ZCR_SM_Sculpting)
        {
            if (this->isActiveOnScene)
            {
                dscSurface.SetUniformsData(ZC_UN_unAlpha, &alphaOne);
                dscSurface.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_Drawing);
            }
            else dscSurface.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_None);
        }
        else
        {
            if (this->isActiveOnScene)
            {
                dscSurface.SetUniformsData(ZC_UN_unAlpha, &alpha);
                dscSurface.SwitchToDrawLvl(ZC_RL_Default, ZCR_DL_AlphaBlending);
            }
            else
            {
                dscSurface.SetUniformsData(ZC_UN_unAlpha, &alphaOne);
                dscSurface.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_Drawing);
            };
        }
    }
    else    //  alpha = 1.f
    {
        dscSurface.SetUniformsData(ZC_UN_unAlpha, &alphaOne);
        switch (sceneMode)
        {
        case ZCR_SM_None: dscSurface.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_Drawing); break;
        case ZCR_SM_Model: dscSurface.SwitchToDrawLvl(ZC_RL_Default, this->isActiveOnScene ? ZC_DL_StencilBorder : ZC_DL_Drawing); break;
        case ZCR_SM_Edit: dscSurface.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_Drawing); break;
        case ZCR_SM_Sculpting: dscSurface.SwitchToDrawLvl(ZC_RL_Default, this->isActiveOnScene ? ZC_DL_Drawing : ZC_DL_None); break;
        }
    }
}

void ZCR_Surface::SetAlpha(float _alpha)
{
    if (alpha == _alpha) return;
    alpha = _alpha < 0.f ? 0.f
        :_alpha > 1.f ? 1.f
        : _alpha;

    ChangeSceneModeTriangle(ZCR_IGWBM_Mode::GetActiveSceneMode());
}

float ZCR_Surface::GetAlpha()
{
    return alpha;
}

void ZCR_Surface::SetUseLight(bool _useLight)
{
    useLight = _useLight;
}

bool ZCR_Surface::IsUseLight()
{
    return useLight;
}

ZC_DrawerSet ZCR_Surface::CreateSurfaceDrawerSet()
{
    ulong elementsCount = 0;
    GLenum elementsType = 0;
    ZC_DA<uchar> elements = GetTriangleElements(elementsCount, elementsType);

    ZC_Buffer ebo(GL_ELEMENT_ARRAY_BUFFER);
    ebo.GLNamedBufferData(elements.size, elements.Begin(), GL_STATIC_DRAW);

    auto upDraw = ZC_uptrMakeFromChild<ZC_GLDraw, ZC_DrawElements>(GL_TRIANGLES, elementsCount, elementsType, 0);

    typename ZC_ShProgs::ShPInitSet* pShPIS = ZC_ShProgs::Get(ShPN_ZCR_ColorFigure);

    ZC_VAO vao;
    vao.Config(pShPIS->vaoConfigData, vbo, &ebo, 0, (quads.size * 4) + (triangles.size * 3));   //  vertices count in vbo (in one quad 4, in one triangle 3)

    std::forward_list<ZC_Buffer> buffers;
    buffers.emplace_front(std::move(ebo));

    return { pShPIS, std::move(vao), std::move(upDraw), std::move(buffers) };
    // switch (element)
    // {
    // case GLElement::Triangle: return { new ZC_RSNonTex(pShPIS, std::move(vao), std::move(upDraw), std::move(buffers)) };
    // case GLElement::Point: return { new ZC_RSNonTex(pShPIS, std::move(vao), std::move(upDraw), std::move(buffers)) };
    // case GLElement::Line: return { new ZC_RSNonTex(pShPIS, std::move(vao), std::move(upDraw), std::move(buffers)) };
    // }
}

ZC_DSController ZCR_Surface::MakeSurfaceRSController()
{
    // std::forward_list<ZC_uptr<ZC_RSPersonalData>>{ZC_RSPDStencilBorder::Make({ 1.05f, ZC_PackColorUCharToUInt(255, 90, 0) })};
    // ZC_RSPDStencilBorder::Make({ 1.05f, ZC_PackColorUCharToUInt(255, 90, 0) });

    std::forward_list<ZC_uptr<ZC_RSPersonalData>> persDatas;
    persDatas.emplace_front(ZC_RSPDStencilBorder::Make({ 1.05f, 1.05f, 1.05f, ZC_PackColorUCharToUInt_RGB(255, 90, 0) }));    //  stencil border color {r = 255, g = 90, b = 0}
    return dsSurface.MakeZC_DSController(-1, std::move(persDatas));
}

ZC_DA<uchar> ZCR_Surface::GetTriangleElements(ulong& rElementsCount, GLenum& rElementsType)
{
    ulong quadsElementsCount = quads.size * 6,     //  6 elements in ebo on one quad
        trianglesElementsCount = triangles.size * 3;     //  3 elements in ebo on one triangle
    rElementsCount = quadsElementsCount + trianglesElementsCount;  
    ulong verticesInVBO = (quads.size * 4) + trianglesElementsCount,     //  4 vertices in vbo on one quad
        storingTypeSize = 0;
    ZC_Buffer::GetElementsData(verticesInVBO - 1, storingTypeSize, rElementsType);
    ZC_DA<uchar> elements(storingTypeSize * rElementsCount);
    switch (storingTypeSize)
    {
    case 1: FillTriangleElements<uchar>(elements.pHead, elements.size, quadsElementsCount); break;
    case 2: FillTriangleElements<ushort>(reinterpret_cast<ushort*>(elements.pHead), elements.size / 2, quadsElementsCount); break;
    case 4: FillTriangleElements<uint>(reinterpret_cast<uint*>(elements.pHead), elements.size / 4, quadsElementsCount); break;
    }
    return elements;
}