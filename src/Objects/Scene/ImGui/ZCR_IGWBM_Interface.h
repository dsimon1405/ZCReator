#pragma once

#include <ZC_IGWBM.h>
#include <Objects/Scene/Orientation3D/ZCR_Orientation3D.h>

class ZCR_IGWBM_Interface : public ZC_IGWBM
{
public:
    ZCR_IGWBM_Interface();

private:
    // ZCR_Orientation3D orientatin3D;
    bool showInterface = true;

    void VCallButtonDownBM(ZC_ButtonID buttonId, float time) override;
    void VDrawWindowIGW() override;

    void DrawInterface(bool isActiveBM);
    void DrawMeshOptions(bool isActiveBM);
    void DrawViewportOptions();
};