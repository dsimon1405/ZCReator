#pragma once

#include <ZC_IGWBM.h>
#include "ZCR_IGWBM_Create.h"
#include "ZCR_IGWBM_Interface.h"
#include "ZCR_IGWBM_Mode.h"

class ZCR_IGWBM_MainMenu : public ZC_IGWBM
{
public:

    ZCR_IGWBM_MainMenu();

private:
    static inline float orthoIndentX = 10.f,
        orthoIndentY = 30;

    ZCR_IGWBM_Create igwbmCreate;
    ZCR_IGWBM_Interface igwbmInterface;
    ZCR_IGWBM_Mode igwbmMode;

    void VDrawWindowIGW() override;
    void VCallButtonDownBM(ZC_ButtonID buttonId, float time) override;
};