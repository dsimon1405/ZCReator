#include "ZCR_Line.h"

ZCR_Line::ZCR_Line()
    : spRendererSetsLine(MakeLineRendererSet()),
    spRSADSLine(spRendererSetsLine->Make_sptrRendererSetDrawingSet(nullptr, 0, 0))
{}

ZC_sptr<ZC_RendererSet> ZCR_Line::MakeLineRendererSet()
{
    size_t elementsCount = 0;
    GLenum elementsType = 0;
    ZC_DA<uchar> elements = GetLineElements(elementsCount, elementsType);

    ZC_Buffer ebo(GL_ELEMENT_ARRAY_BUFFER);
    ebo.BufferData(elements.size, elements.Begin(), GL_STATIC_DRAW);

    auto upDraw = ZC_uptrMakeFromChild<ZC_GLDraw, ZC_DrawElements>(GL_LINES, elementsCount, elementsType, 0);

    typename ZC_ShProgs::ShPInitSet* pShPIS = ZC_ShProgs::Get(ZC_ShProgs::Name::ZCR_Line);

    ZC_VAO vao;                                        //  vertices count in vbo (in one quad 4, in one triangle 3)
    vao.Config(pShPIS->vaoConData, *vbo, &ebo, 0, (spQuads ? (spQuads->size * 4) : 0) + (spTriangles ? (spTriangles->size * 3) : 0));

    std::forward_list<ZC_Buffer> buffers;
    buffers.emplace_front(std::move(ebo));

    return { new ZC_RSNonTex(pShPIS, std::move(vao), std::move(upDraw), std::move(buffers)) };
}

ZC_DA<uchar> ZCR_Line::GetLineElements(size_t& rElementsCount, GLenum& rElementsType)
{
    std::forward_list<Lines> lines = GetLines(rElementsCount);
    size_t storingTypeSize = 0;
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

std::forward_list<typename ZCR_Line::Lines> ZCR_Line::GetLines(size_t& rElementsCount)
{
    typename std::forward_list<Lines> lines;

    auto linesAdd = [&lines](ZC_Vec3<float>& corner1, ZC_Vec3<float>& corner2, bool isQuad)
    {
        auto edgeCenter = (corner1 + corner2) / 2.f;
        auto pLines = std::find(lines.begin(), lines.end(), edgeCenter);
        if (pLines == lines.end())
        {
            std::forward_list<typename Lines::Corners> corners;
            corners.emplace_front(&corner1, &corner2, isQuad);
            lines.emplace_front(Lines{ edgeCenter, std::move(corners) });
            return 2;   //  in ebo line is two values, if not found same return 2 (new line)
        }
        else
        {
            pLines->corners.emplace_front(&corner1, &corner2, isQuad);
            return 0;   //  in ebo line is two values, if found same return 0
        }
    };
    for (size_t i = 0; i < (spTriangles ? spTriangles->size : 0); ++i)
    {
        rElementsCount += linesAdd((*spTriangles)[i].bl, (*spTriangles)[i].tc, false);
        rElementsCount += linesAdd((*spTriangles)[i].tc, (*spTriangles)[i].br, false);
        rElementsCount += linesAdd((*spTriangles)[i].br, (*spTriangles)[i].bl, false);
    }
    for (size_t i = 0; i < (spQuads ? spQuads->size : 0); ++i)
    {
        rElementsCount += linesAdd((*spQuads)[i].bl, (*spQuads)[i].tl, true);
        rElementsCount += linesAdd((*spQuads)[i].tl, (*spQuads)[i].tr, true);
        rElementsCount += linesAdd((*spQuads)[i].tr, (*spQuads)[i].br, true);
        rElementsCount += linesAdd((*spQuads)[i].br, (*spQuads)[i].bl, true);
    }
    return lines;
}

void ZCR_Line::SwitchRSandDSLine(RSLvl lvl)
{
    spRSADSLine->SwitchToLvl(lvl);
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