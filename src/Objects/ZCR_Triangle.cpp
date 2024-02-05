#include "ZCR_Triangle.h"

#include <ZC/Video/OpenGL/Renderer/ZC_Renderer.h>
#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>

ZCR_Triangle::ZCR_Triangle(ZC_DA<ZC_Quad>&& _quads, ZC_DA<unsigned char>& colors, ZC_DA<int>&& _normals, const ZC_Mat4<float>& model)
    : quads(std::move(_quads)),
    normals(std::move(_normals)),
    vbo(GL_ARRAY_BUFFER),
    rendSet(RendSet(colors))
{
    pUniforms = rendSet.Add();
    pUniforms->Set(ZC_Uniform::Name::model, const_cast<ZC_Mat4<float>&>(model).Begin());

    rendSet.AddStencil(pUniforms, {1.f, 1.f, 0.f});
}

ZCR_Triangle::ZCR_Triangle(ZCR_Triangle&& tr)
    : quads(std::move(tr.quads)),
    normals(std::move(tr.normals)),
    vbo(std::move(tr.vbo)),
    rendSet(std::move(rendSet)),
    pUniforms(tr.pUniforms)
{}

ZC_RSNonTex ZCR_Triangle::RendSet(ZC_DA<unsigned char>& colors)
{
    auto pShPIS = ZC_ShProgs::Get(ZC_ShProgs::Name::Color);
    
    auto qBSize = quads.BytesSize(),
        cBSize = colors.BytesSize(),
        nBSize = normals.BytesSize(),
        qcBSize = qBSize + cBSize;
    vbo.BufferData(qcBSize + nBSize, &(quads.Begin()->bl), GL_DYNAMIC_DRAW);
    vbo.BufferSubData(qBSize, cBSize, colors.Begin());
    vbo.BufferSubData(qcBSize, nBSize, normals.Begin());
    size_t elementsCount = 0;
    GLenum elementsType;
    auto elements = GetElements(quads.size, elementsCount, elementsType);
    ZC_Buffer ebo(GL_ELEMENT_ARRAY_BUFFER);
    ebo.BufferData(elements.size, elements.Begin(), GL_STATIC_DRAW);
    auto upDraw = ZC_uptrMakeFromChild<ZC_Draw, ZC_DrawElements>(GL_TRIANGLES, elementsCount, elementsType, 0);
    ZC_VAO vao;
    vao.Config(pShPIS->pVaoCon, vbo, ebo, 0, quads.size * 4);   //  4 vertices in one quad
    std::forward_list<ZC_Buffer> buffers;
    buffers.emplace_front(std::move(ebo));
    return ZC_RSNonTex(pShPIS, std::move(vao), std::move(upDraw), std::move(buffers));
}

ZC_DA<unsigned char> ZCR_Triangle::GetElements(size_t quadsCount, size_t& rElementsCount, GLenum& rElementsType)
{
    rElementsCount = quadsCount * 6;  //  6 ebo's elements on one quad
    size_t maxIndex = quadsCount * 4 - 1,  //  4 indices on one quad
        storingTypeSize = 0;
    ZC_Buffer::GetIndexData(maxIndex, storingTypeSize, rElementsType);
    ZC_DA<unsigned char> indices(storingTypeSize * rElementsCount);
    switch (storingTypeSize)
    {
    case 1: FillElements(indices.pHead, indices.size); break;
    case 2: FillElements(reinterpret_cast<unsigned short*>(indices.pHead), indices.size / 2); break;
    case 4: FillElements(reinterpret_cast<unsigned int*>(indices.pHead), indices.size / 4); break;
    }
    return indices;
}
