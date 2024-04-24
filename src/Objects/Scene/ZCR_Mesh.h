#pragma once

#include <ZC/Video/OpenGL/Renderer/ZC_RenderSet.h>
#include "ZCR_Axis.h"

struct ZCR_Mesh
{

    ZCR_Mesh(float meshLength);
    void SetAxis(ZCR_Axis axis);

private:
    enum CoordSystem
    {
        none,
        xy,
        xz,
        yz
    };

    CoordSystem coordSystem { CoordSystem::none };
    ZC_uptr<ZC_RenderSet> upRendererSet;
    ZC_RSController rsController_axisXY;
    ZC_RSController rsController_axisXZ;
    ZC_RSController rsController_axisYZ;
    
    ZC_uptr<ZC_RenderSet> MakeRendererSet(float totalLength);
    void SwitchToCoordSystem(CoordSystem _coordSystem);
};