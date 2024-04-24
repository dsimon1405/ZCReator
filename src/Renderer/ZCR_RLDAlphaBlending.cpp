#include "ZCR_RLDAlphaBlending.h"

#include <cassert>

void ZCR_RLDAlphaBlending::VAdd(ZC_RSController* pRSController)
{
    assert(fl.Empty());     // only one object can be at that level
    fl.Add(pRSController->pShProg, { pRSController->pTexture, pRSController->texturesCount }, pRSController->pVAO,
        { static_cast<const ZC_Uniforms*>(pRSController->GetPersonalData(ZC_RSPDC_uniforms)), pRSController->pGLDraw });
}

bool ZCR_RLDAlphaBlending::VErase(ZC_RSController* pRSController)
{
    return true;    //  stores only one object, causes level deletion
}