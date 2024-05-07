#pragma once

#include <ZC_IGWBM.h>

struct ZCR_IGWBM_M_Model : public ZC_IGWBM
{
public:
    ZCR_IGWBM_M_Model();

private:
    void VDrawWindowIGW() override;
    void VCallButtonDownBM(ZC_ButtonID buttonId, float time) override;
    void VDeactivateIGWBM() override;
};