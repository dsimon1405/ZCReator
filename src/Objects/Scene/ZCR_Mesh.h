#pragma once

#include <ZC/Video/OpenGL/Renderer/ZC_RendererSet.h>

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
    ZC_RSController rsController_axisXY;
    ZC_RSController rsController_axisXZ;
    ZC_RSController rsController_axisYZ;
    
    ZC_uptr<ZC_RendererSet> MakeRendererSet(float totalLength);
};