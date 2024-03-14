#pragma once

#include <ZC/Tools/ZC_sptr.h>
#include <ZC/Tools/Container/ZC_DA.h>
#include <ZC/Tools/Math/ZC_Figures.h>
#include <ZC/Video/OpenGL/Renderer/ZC_RSNotTextured.h>
#include <ZC/Tools/Math/ZC_Vec2.h>

struct ZCR_FigureData
{
    virtual ~ZCR_FigureData() = default;
    
protected:
    ZCR_FigureData() = default;
    ZCR_FigureData(ZC_sptr<ZC_DA<ZC_Quad>>&& spQuads, ZC_sptr<ZC_DA<ZC_Triangle>>&& spTriangles, ZC_sptr<ZC_DA<int>>&& spNormals);

    enum GLElement
    {
        Triangle = 0,
        Point,
        Line
    };

    ZC_sptr<ZC_DA<ZC_Quad>> spQuads;
    ZC_sptr<ZC_DA<ZC_Triangle>> spTriangles;
    ZC_DA<ZC_Vec3<uchar>> colors;
    ZC_sptr<ZC_DA<int>> spNormals;
    ZC_sptr<ZC_DA<ZC_Vec2<float>>> spTexCoords = nullptr;

    ZC_sptr<ZC_RendererSet> MakeRendererSet(GLElement elementsType);
};