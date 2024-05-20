#pragma once

#include <ZC_IGWBM.h>

#include <ZC_IGInputText.h>
#include <iostream>

struct ZCR_IGWBM_M_Model : public ZC_IGWBM, public ZC_IGInputText
{
public:
    ZCR_IGWBM_M_Model();

    void VLostFocuseIGIT(ZC_DA<char>& buffer) override
    {
    std::cout<<"lost focus III"<<std::endl;
        
    }

private:
    void VDrawWindowIGW() override;
    void VCallButtonDownBM(ZC_ButtonID buttonId, float time) override;
    void VDeactivateIGWBM() override;
};