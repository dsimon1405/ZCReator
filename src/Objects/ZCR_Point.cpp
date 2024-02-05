#include "ZCR_Point.h"

#include <ZC/Video/OpenGL/Renderer/ZC_Renderer.h>
#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>

ZCR_Point::ZCR_Point(ZC_DA<ZC_Quad>& quads, ZC_Buffer& vbo, const ZC_Mat4<float>& model)
    : rendSet(MakeRendSet(quads, vbo))
{
    pUniforms = rendSet.Add();
    pUniforms->Set(ZC_Uniform::Name::model, const_cast<ZC_Mat4<float>&>(model).Begin());
}

ZCR_Point::ZCR_Point(ZCR_Point&& p)
    : rendSet(std::move(p.rendSet)),
    pUniforms(p.pUniforms)
{}

ZC_RSNonTex ZCR_Point::MakeRendSet(ZC_DA<ZC_Quad>& quads, ZC_Buffer& vbo)
{
    auto pShPIS = ZC_ShProgs::Get(ZC_ShProgs::Name::Point);
    
    size_t elementsCount = 0;
    GLenum elementsType;
    auto elements = GetElements(quads, elementsCount, elementsType);

    ZC_Buffer ebo(GL_ELEMENT_ARRAY_BUFFER);
    ebo.BufferData(elements.size, elements.pHead, GL_STATIC_DRAW);

    auto upDraw = ZC_uptrMakeFromChild<ZC_Draw, ZC_DrawElements>(GL_POINTS, elementsCount, elementsType, 0);
    
    ZC_VAO vao;
    vao.Config(pShPIS->pVaoCon, vbo, ebo, 0, quads.size * 4);
    
    std::forward_list<ZC_Buffer> buffers;
    buffers.emplace_front(std::move(ebo));

    return ZC_RSNonTex(pShPIS, std::move(vao), std::move(upDraw), std::move(buffers), 1);
}

ZC_DA<unsigned char> ZCR_Point::GetElements(ZC_DA<ZC_Quad>& quads, size_t& rElementsCount, GLenum& rElementsType)
{
    typename std::list<Point> points = GetPoints(quads);
    rElementsCount = points.size();
    size_t storingTypeSize = 0;
    ZC_Buffer::GetIndexData(rElementsCount - 1, storingTypeSize, rElementsType);
    ZC_DA<unsigned char> indices(storingTypeSize * rElementsCount);
    switch (storingTypeSize)
    {
    case 1: FillElements(points, indices.pHead, &(quads.Begin()->bl)); break;
    case 2: FillElements(points, reinterpret_cast<unsigned short*>(indices.pHead), &(quads.Begin()->bl)); break;
    case 4: FillElements(points, reinterpret_cast<unsigned int*>(indices.pHead), &(quads.Begin()->bl)); break;
    }
    return indices;
}

typename std::list<typename ZCR_Point::Point> ZCR_Point::GetPoints(ZC_DA<ZC_Quad>& quads)
{
    std::list<Point> points;
    
    auto add = [&points](ZC_Vec3<float>& vertex)
    {
        auto iter = std::find(points.begin(), points.end(), vertex);
        if (iter == points.end())
            points.emplace_back(Point{{&vertex}});
        else iter->samePoints.emplace_front(&vertex);
    };
    for (size_t i = 0; i < quads.size; ++i)
    {
        add(quads[i].bl);
        add(quads[i].tl);
        add(quads[i].tr);
        add(quads[i].br);
    }
    return points;
}


//  Point start


bool ZCR_Point::Point::operator == (const ZC_Vec3<float>& _v) const noexcept
{
    return **samePoints.begin() == _v;
}

ZC_Vec3<float>* ZCR_Point::Point::GetPointerOnPoint()
{
    return *samePoints.begin();
}