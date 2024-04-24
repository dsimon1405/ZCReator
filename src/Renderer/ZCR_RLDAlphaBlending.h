#pragma once

#include <ZC/Video/OpenGL/Renderer/ZC_RLDText.h>

struct ZCR_RLDAlphaBlending : public ZC_RLDTextScene
{
    void VAdd(ZC_RSController* pRSController) override;
    bool VErase(ZC_RSController* pRSController) override;
};