#include "ZCR_Figure.h"

ZCR_Figure::ZCR_Figure(ZC_DA<ZC_Quad>&& quads, ZC_DA<unsigned char>&& _colors, ZC_DA<int>&& normals, const ZC_Mat4<float>& model)
    : colors(std::move(_colors)),
    triangles(std::move(quads), colors, std::move(normals), model),
    points(triangles.quads, triangles.vbo, model),
    lines(triangles.quads, triangles.vbo, model)
{}

ZCR_Figure::ZCR_Figure(ZCR_Figure&& fig)
    : colors(std::move(fig.colors)),
    triangles(std::move(fig.triangles)),
    points(std::move(fig.points)),
    lines(std::move(fig.lines))
{}

ZC_DA<unsigned char> ZCR_Figure::BlackColors(size_t count)
{
    ZC_DA<unsigned char> colors(count);
    std::fill(colors.pHead, colors.pHead + count, 0);
    return colors;
}