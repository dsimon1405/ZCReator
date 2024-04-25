#pragma once

#include <ZC/Video/OpenGL/Renderer/ZC_RenderSet.h>
#include "ZCR_Axis.h"

class ZCR_Mesh
{
public:
    enum CoordSystem
    {
        CS_None,    //  don't draw mesh
        CS_XY,
        CS_XZ,
        CS_YZ,
        CS_Nothing  //  added for specificCoordSystem
    };

    virtual ~ZCR_Mesh() = default;

    void SetMeshSpecificCoordSystem(CoordSystem _coordSystem);

protected:
    ZCR_Mesh(float meshLength);
    
    void SetMeshAxis(ZCR_Axis axis);

private:
    CoordSystem coordSystem { CoordSystem::CS_None };
    CoordSystem specificCoordSystem { CoordSystem::CS_Nothing };
    ZC_uptr<ZC_RenderSet> upRendererSet;
    ZC_RSController rsController_axisXY;
    ZC_RSController rsController_axisXZ;
    ZC_RSController rsController_axisYZ;
    
    ZC_uptr<ZC_RenderSet> MakeRendererSet(float totalLength);
    void SwitchToCoordSystem(CoordSystem _coordSystem);
};