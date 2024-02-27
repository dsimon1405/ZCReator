#pragma once

#include <ZC/Video/OpenGL/Renderer/ZC_RendererSetAndDrawingSet.h>

struct ZCR_Mesh
{
    enum CoordSystem
    {
        none,
        xy,
        xz,
        yz
    };

    ZCR_Mesh(float meshLength);

    void SwitchToCoordSystem(CoordSystem _coordSystem);

private:
    CoordSystem coordSystem { CoordSystem::none };
    ZC_uptr<ZC_RendererSet> upRendererSet;
    ZC_uptr<ZC_RendererSetAndDrawingSet> upRSADSaxisXY;
    ZC_uptr<ZC_RendererSetAndDrawingSet> upRSADSaxisXZ;
    ZC_uptr<ZC_RendererSetAndDrawingSet> upRSADSaxisYZ;
    
    typedef typename ZC_RendererSet::Level RSLvl;

    ZC_uptr<ZC_RendererSet> MakeRendererSet(float totalLength);
};