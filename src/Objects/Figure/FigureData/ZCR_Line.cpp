#include "ZCR_Line.h"

#include <ZC/Tools/Container/ZC_ContFunc.h>

ZCR_Line::ZCR_Line()
    : dsLine(CreateLineDrawerSet()),
    dsConLine(dsLine.MakeZC_DSController())
{}

ZCR_Line::ZCR_Line(ZCR_Line&& l)
    : dsLine(std::move(l.dsLine)),
    dsConLine(dsLine.MakeZC_DSController())
{
    dsConLine.SwitchToDrawLvl(ZC_RL_Default, l.dsConLine.GetDrawingLevel(ZC_RL_Default));   //  switch new controller to acoding level of previous controller
}

ZC_DrawerSet ZCR_Line::CreateLineDrawerSet()
{
    ulong elementsCount = 0;
    GLenum elementsType = 0;
    ZC_DA<uchar> elements = GetLineElements(elementsCount, elementsType);

    ZC_Buffer ebo(GL_ELEMENT_ARRAY_BUFFER);
    ebo.BufferData(elements.size, elements.Begin(), GL_STATIC_DRAW);

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

void ZCR_Line::SwitchRSControllerLine(ZC_DrawerLevel drawerLevel)
{
    dsConLine.SwitchToDrawLvl(ZC_RL_Default, drawerLevel);
}


//  Line start

bool ZCR_Line::Lines::operator == (const ZC_Vec3<float>& _edgeCenter) const noexcept
{
    return edgeCenter == _edgeCenter;
}

typename ZCR_Line::Lines::Corners& ZCR_Line::Lines::GetCorners()
{
    return *corners.begin();
}