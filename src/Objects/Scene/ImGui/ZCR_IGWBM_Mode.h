#pragma once

#include <Objects/Scene/ZCR_SceneMode.h>
#include "ZCR_IGWBM_M_Model.h"
#include "ZCR_IGWBM_M_Edit.h"
#include "ZCR_IGWBM_M_Sculpting.h"

struct ZCR_IGWBM_Mode : public ZC_IGWBM
{
public:
    ZCR_IGWBM_Mode(float startX, float startY);
    ~ZCR_IGWBM_Mode();

    static ZCR_SceneMode GetActiveSceneMode() noexcept;
    static void SetActiveSceneMode(ZCR_SceneMode _sceneMode, bool changeFiguresState) noexcept;
    static bool IsOneOfModes_ActiveBM() noexcept;
    static void ActivateIGWBM();

private:
    static inline ZCR_IGWBM_Mode* pIGWBM_Mode;

    ZCR_SceneMode activeSceneMode = ZCR_SM_None;
    ZCR_IGWBM_M_Model igwbmModel;
    ZCR_IGWBM_M_Edit igwbmEdit;
    ZCR_IGWBM_M_Sculpting igwbmSculpting;

    void VDrawWindowIGW() override;
    void VCallButtonDownBM(ZC_ButtonID buttonId, float time) override;

    void ChangeDrawState(ZCR_SceneMode sceneMode, bool needDraw);
};