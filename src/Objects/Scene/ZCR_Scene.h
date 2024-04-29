#pragma once

#include "ZCR_SceneMode.h"
#include <Objects/Scene/ImGui/ZCR_IGWCreateAndMode.h>
#include "ZCR_Mesh.h"
#include "ZCR_Camera.h"

class ZCR_Scene : public ZCR_Mesh, public ZCR_Camera
{
public:
    ZCR_Scene();
    ~ZCR_Scene();

    static ZCR_Scene* GetScene() noexcept;

    ZCR_SceneModes GetActiveSceneMode() const noexcept;
    void SetActiveSceneMode(ZCR_SceneModes _sceneMode, bool changeFiguresState) noexcept;
    void SetAxise(ZCR_Axis axis);
    ZCR_Axis GetAxis() const noexcept;

private:
    static inline ZCR_Scene* pScene;

    ZCR_SceneModes activeSceneMode = ZCR_SM_Model;
    ZCR_IGWCreateAndMode igwCreateAndMode;
    ZCR_Axis axis = ZCR_A_Free;
    // ZC_uptr<ZCR_SceneMode> sceneModes[3];
};