#pragma once

#include <ZC/Video/OpenGL/Renderer/ZC_DrawerSet.h>
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
        CS_Auto  //  added for specificCoordSystem
    };

    virtual ~ZCR_Mesh() = default;

    void SetMeshSpecificCoordSystem(CoordSystem _coordSystem);
    CoordSystem GetMeshSpecificCoordSystem();

protected:
    ZCR_Mesh(float meshLength);
    
    void SetMeshAxis(ZCR_Axis axis);

private:
    CoordSystem coordSystem { CoordSystem::CS_None };
    CoordSystem specificCoordSystem { CoordSystem::CS_Auto };
    ZC_DrawerSet drawerSet;
    ZC_DSController dsController_axisXY;
    ZC_DSController dsController_axisXZ;
    ZC_DSController dsController_axisYZ;
    
    ZC_DrawerSet CreateDrawerSet(float totalLength);
    void SwitchToCoordSystem(CoordSystem _coordSystem);
};