#include "ZCR_Line.h"

#include <ZC/Video/OpenGL/Renderer/ZC_Renderer.h>
#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>

ZCR_Line::ZCR_Line(ZC_DA<ZC_Quad>& quads, ZC_Buffer& vbo, const ZC_Mat4<float>& model)
    : rendSet(RendSet(quads, vbo))
{
    pUniforms = rendSet.Add();
    pUniforms->Set(ZC_Uniform::Name::model, const_cast<ZC_Mat4<float>&>(model).Begin());
}

ZCR_Line::ZCR_Line(ZCR_Line&& p)
    : rendSet(std::move(p.rendSet)),
    pUniforms(p.pUniforms)
{}

ZC_RSNonTex ZCR_Line::RendSet(ZC_DA<ZC_Quad>& quads, ZC_Buffer& vbo)
{
    auto pShPIS = ZC_ShProgs::Get(ZC_ShProgs::Name::Line);
    
    size_t elementsCount = 0;
    GLenum elementsType;
    auto elements = GetElements(quads, elementsCount, elementsType);
    ZC_Buffer ebo(GL_ELEMENT_ARRAY_BUFFER);
    ebo.BufferData(elements.size, elements.pHead, GL_STATIC_DRAW);
    auto upDraw = ZC_uptrMakeFromChild<ZC_Draw, ZC_DrawElements>(GL_LINES, elementsCount, elementsType, 0);
    ZC_VAO vao;
    vao.Config(pShPIS->pVaoCon, vbo, ebo, 0, quads.size * 4);
    std::forward_list<ZC_Buffer> buffers;
    buffers.emplace_front(std::move(ebo));
    return ZC_RSNonTex(pShPIS, std::move(vao), std::move(upDraw), std::move(buffers), 1);
}

ZC_DA<unsigned char> ZCR_Line::GetElements(ZC_DA<ZC_Quad>& quads, size_t& rElementsCount, GLenum& rElementsType)
{
    typename std::list<Line> lines = GetLines(quads);
    rElementsCount = lines.size() * 2;
    size_t storingTypeSize = 0;
    ZC_Buffer::GetIndexData(rElementsCount - 1, storingTypeSize, rElementsType);
    ZC_DA<unsigned char> elements(storingTypeSize * rElementsCount);
    switch (storingTypeSize)
    {
    case 1: FillElements(lines, elements.pHead, &(quads.Begin()->bl)); break;
    case 2: FillElements(lines, reinterpret_cast<unsigned short*>(elements.pHead), &(quads.Begin()->bl)); break;
    case 4: FillElements(lines, reinterpret_cast<unsigned int*>(elements.pHead), &(quads.Begin()->bl)); break;
    }
    return elements;
}

typename std::list<typename ZCR_Line::Line> ZCR_Line::GetLines(ZC_DA<ZC_Quad>& quads)
{
    typename std::list<Line> lines;
    auto addLine = [&lines](ZC_Vec3<float>& corner1, ZC_Vec3<float>& corner2)
    {
        auto edgeCenter = (corner1 + corner2) / 2.f;
        auto linesIter = std::find(lines.begin(), lines.end(), edgeCenter);
        if (linesIter == lines.end())
        {
            std::forward_list<typename Line::Corners> corners;
            corners.emplace_front(&corner1, &corner2);
            lines.emplace_back(Line{edgeCenter, std::move(corners)});
        }
        else linesIter->corners.emplace_front(&corner1, &corner2);
    };
    for (size_t i = 0; i < quads.size; ++i)
    {
        addLine(quads[i].bl, quads[i].tl);
        addLine(quads[i].tl, quads[i].tr);
        addLine(quads[i].tr, quads[i].br);
        addLine(quads[i].br, quads[i].bl);
    }
    return lines;
}


//  Line start

bool ZCR_Line::Line::operator == (const ZC_Vec3<float>& _edgeCenter) const noexcept
{
    return edgeCenter == _edgeCenter;
}