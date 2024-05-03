#pragma once

#include <ZC_IGWBM.h>
#include <ZC/Tools/Signal/ZC_SConnection.h>
#include <Objects/Scene/Orientation3D/ZCR_Orientation3D.h>

class ZCR_IGWScene : public ZC_IGWBM
{
public:
    ZCR_IGWScene();

private:
    ZCR_Orientation3D orientatin3D;
    bool checkBoxIsOrienttion3D = true;

    void VDrawWindowIGW() override;
    void VCallButtonDownBM(ZC_ButtonID buttonId, float time) override;  //  here heir must realize switch for all necessary methods
};