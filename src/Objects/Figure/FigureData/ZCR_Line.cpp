#include "ZCR_Line.h"

#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <Objects/Scene/ImGui/ZCR_IGWBM_Mode.h>

bool ZCR_Line::IsDrawLines()
{
    return drawLines;
}

void ZCR_Line::SetDrawLines(bool _drawLines)
{
    if (drawLines == _drawLines) return;
    drawLines = _drawLines;
    ChangeSceneModeLine(ZCR_IGWBM_Mode::GetActiveSceneMode());
}

ZCR_Line::ZCR_Line()
    : dsLine(CreateLineDrawerSet()),
    dscLine(dsLine.MakeZC_DSController())
{}

// ZCR_Line::ZCR_Line(ZCR_Line&& l)
//     : dsLine(std::move(l.dsLine)),
//     dsConLine(dsLine.MakeZC_DSController())
// {
//     dsConLine.SwitchToDrawLvl(ZC_RL_Default, l.dsConLine.GetDrawingLevel(ZC_RL_Default));   //  switch new controller to acoding level of previous controller
// }

void ZCR_Line::ChangeSceneModeLine(ZCR_SceneMode sceneMode)
{
    if (sceneMode == ZCR_SM_Edit && isActiveOnScene)
    {
        if (colorStateGPU == CS_GPU_AllPassive) this->FillColorActivePoints(this->activePoints, false, true);
        dscLine.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_Drawing);
    }
    else if (drawLines)
    {
        if (colorStateGPU != CS_GPU_AllPassive) this->FillColorsAllAsPassive(true);
        dscLine.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_Drawing);      //  herer add stencilBorder level
    }
    else dscLine.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_None);

    // switch (sceneMode)
    // {
    // case ZCR_SM_None:
    // {
    //     if (isActiveOnScene)
    //     {
    //         dscLine.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_None);
    //     }
    //     else
    //     {
    //         dscLine.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_None);
    //     };
    // } break;
    // case ZCR_SM_Model:
    // {
    //     if (isActiveOnScene)
    //     {
    //         dscLine.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_None);
    //     }
    //     else
    //     {
    //         dscLine.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_None);
    //     };
    // } break;
    // case ZCR_SM_Edit:
    // {
    //     if (isActiveOnScene)
    //     {
    //         dscLine.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_Drawing);
    //     }
    //     else
    //     {
    //         dscLine.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_None);
    //     };
    // } break;
    // case ZCR_SM_Sculpting:
    // {
    //     if (isActiveOnScene)
    //     {
    //         dscLine.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_None);
    //     }
    //     else
    //     {
    //         dscLine.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_None);
    //     };
    // } break;
    // }
}




// void ZCR_Figure::ChangeSceneMode(ZCR_SceneMode sceneMode)
// {
//     if (alpha == 0.f) sceneMode == ZCR_SM_Edit ?
//         SwitchGLElementOnRendererLevel(ZC_DL_None, ZC_DL_Drawing, ZC_DL_Drawing) : SwitchGLElementOnRendererLevel(ZC_DL_None, ZC_DL_None, ZC_DL_None);
//     else if (alpha < 1.f)
//     {
//         switch (sceneMode)
//         {
//         case ZCR_SM_None:
//         {
//             if (isActiveOnScene)
//             {
//                 dsConSurface.SetUniformsData(ZC_UN_unAlpha, &alpha);
//                 SwitchGLElementOnRendererLevel(ZCR_DL_AlphaBlending, ZC_DL_None, ZC_DL_None);
//             }
//             else
//             {
//                 dsConSurface.SetUniformsData(ZC_UN_unAlpha, &alphaOne);
//                 SwitchGLElementOnRendererLevel(ZC_DL_Drawing, ZC_DL_None, ZC_DL_None);
//             };
//         } break;
//         case ZCR_SM_Model:
//         {
//             if (isActiveOnScene)
//             {
//                 dsConSurface.SetUniformsData(ZC_UN_unAlpha, &alpha);
//                 SwitchGLElementOnRendererLevel(ZCR_DL_AlphaBlending, ZC_DL_None, ZC_DL_None);
//             }
//             else
//             {
//                 dsConSurface.SetUniformsData(ZC_UN_unAlpha, &alphaOne);
//                 SwitchGLElementOnRendererLevel(ZC_DL_Drawing, ZC_DL_None, ZC_DL_None);
//             };
//         } break;
//         case ZCR_SM_Edit:
//         {
//             if (isActiveOnScene)
//             {
//                 dsConSurface.SetUniformsData(ZC_UN_unAlpha, &alpha);
//                 SwitchGLElementOnRendererLevel(ZCR_DL_AlphaBlending, ZC_DL_Drawing, ZC_DL_Drawing);
//             }
//             else
//             {
//                 dsConSurface.SetUniformsData(ZC_UN_unAlpha, &alphaOne);
//                 SwitchGLElementOnRendererLevel(ZC_DL_Drawing, ZC_DL_None, ZC_DL_None);
//             };
//         } break;
//         case ZCR_SM_Sculpting:
//         {
//             if (isActiveOnScene)
//             {
//                 dsConSurface.SetUniformsData(ZC_UN_unAlpha, &alphaOne);
//                 SwitchGLElementOnRendererLevel(ZC_DL_Drawing, ZC_DL_None, ZC_DL_None);
//             }
//             else
//             {
//                 SwitchGLElementOnRendererLevel(ZC_DL_None, ZC_DL_None, ZC_DL_None);
//             };
//         } break;
//         }
//     }
//     else
//     {
//         dsConSurface.SetUniformsData(ZC_UN_unAlpha, &alphaOne);
//         switch (sceneMode)
//         {
//         case ZCR_SM_None: SwitchGLElementOnRendererLevel(ZC_DL_Drawing, ZC_DL_None, ZC_DL_None); break;
//         case ZCR_SM_Model: SwitchGLElementOnRendererLevel(isActiveOnScene ? ZC_DL_StencilBorder : ZC_DL_Drawing, ZC_DL_None, ZC_DL_None); break;
//         case ZCR_SM_Edit: SwitchGLElementOnRendererLevel(ZC_DL_Drawing, ZC_DL_Drawing, ZC_DL_Drawing); break;
//         case ZCR_SM_Sculpting: SwitchGLElementOnRendererLevel(isActiveOnScene ? ZC_DL_Drawing : ZC_DL_None, ZC_DL_None, ZC_DL_None); break;
//         }
//     }
// }

ZC_DrawerSet ZCR_Line::CreateLineDrawerSet()
{
    ulong elementsCount = 0;
    GLenum elementsType = 0;
    ZC_DA<uchar> elements = GetLineElements(elementsCount, elementsType);

    ZC_Buffer ebo(GL_ELEMENT_ARRAY_BUFFER);
    ebo.GLNamedBufferData(elements.size, elements.Begin(), GL_STATIC_DRAW);

    auto upDraw = ZC_uptrMakeFromChild<ZC_GLDraw, ZC_DrawElements>(GL_LINES, elementsCount, elementsType, 0);

    typename ZC_ShProgs::ShPInitSet* pShPIS = ZC_ShProgs::Get(ShPN_ZCR_LineFigure);

    ZC_VAO vao;
    vao.Config(pShPIS->vaoConfigData, vbo, &ebo, 0, (quads.size * 4) + (triangles.size * 3));   //  vertices count in vbo (in one quad 4, in one triangle 3)

    std::forward_list<ZC_Buffer> buffers;
    buffers.emplace_front(std::move(ebo));

    return { pShPIS, std::move(vao), std::move(upDraw), std::move(buffers) };
}

ZC_DA<uchar> ZCR_Line::GetLineElements(ulong& rElementsCount, GLenum& rElementsType)
{
    std::forward_list<Lines> lines = GetLines(rElementsCount);
    ulong storingTypeSize = 0;
    ZC_Buffer::GetElementsData(rElementsCount - 1, storingTypeSize, rElementsType);
    ZC_DA<uchar> elements(storingTypeSize * rElementsCount);
    switch (storingTypeSize)
    {
    case 1: FillLineElements(lines, elements.pHead); break;
    case 2: FillLineElements(lines, reinterpret_cast<ushort*>(elements.pHead)); break;
    case 4: FillLineElements(lines, reinterpret_cast<uint*>(elements.pHead)); break;
    }
    return elements;
}

std::forward_list<typename ZCR_Line::Lines> ZCR_Line::GetLines(ulong& rElementsCount)
{
    typename std::forward_list<Lines> lines;

    auto linesAdd = [&lines](ZC_Vec3<float>& corner1, ZC_Vec3<float>& corner2, bool isQuad)
    {
        auto edgeCenter = (corner1 + corner2) / 2.f;
        auto pLines = ZC_Find(lines, edgeCenter);
        if (pLines)
        {
            pLines->corners.emplace_front(&corner1, &corner2, isQuad);
            return 0;   //  in ebo line is two values, if found same return 0
        }
        else
        {
            std::forward_list<typename Lines::Corners> corners;
            corners.emplace_front(&corner1, &corner2, isQuad);
            lines.emplace_front(Lines{ edgeCenter, std::move(corners) });
            return 2;   //  in ebo line is two values, if not found same return 2 (new line)
        }
    };
    for (ulong i = 0; i < triangles.size; ++i)
    {
        rElementsCount += linesAdd(triangles[i].bl, triangles[i].tc, false);
        rElementsCount += linesAdd(triangles[i].tc, triangles[i].br, false);
        rElementsCount += linesAdd(triangles[i].br, triangles[i].bl, false);
    }
    for (ulong i = 0; i < quads.size; ++i)
    {
        rElementsCount += linesAdd(quads[i].bl, quads[i].tl, true);
        rElementsCount += linesAdd(quads[i].tl, quads[i].tr, true);
        rElementsCount += linesAdd(quads[i].tr, quads[i].br, true);
        rElementsCount += linesAdd(quads[i].br, quads[i].bl, true);
    }
    return lines;
}

// ZC_DSController ZCR_Line::MakeSurfaceRSController()
// {
//     // std::forward_list<ZC_uptr<ZC_RSPersonalData>>{ZC_RSPDStencilBorder::Make({ 1.05f, ZC_PackColorUCharToUInt(255, 90, 0) })};
//     // ZC_RSPDStencilBorder::Make({ 1.05f, ZC_PackColorUCharToUInt(255, 90, 0) });

//     std::forward_list<ZC_uptr<ZC_RSPersonalData>> persDatas;
//     persDatas.emplace_front(ZC_RSPDStencilBorder::Make({ 1.05f, 1.05f, 1.05f, ZC_PackColorUCharToUInt(255, 90, 0) }));    //  stencil border color {r = 255, g = 90, b = 0}
//     return dsSurface.MakeZC_DSController(-1, std::move(persDatas));
// }


//  Line start

bool ZCR_Line::Lines::operator == (const ZC_Vec3<float>& _edgeCenter) const noexcept
{
    return edgeCenter == _edgeCenter;
}

typename ZCR_Line::Lines::Corners& ZCR_Line::Lines::GetCorners()
{
    return *corners.begin();
}