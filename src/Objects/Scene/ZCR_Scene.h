#pragma once

#include "ZCR_SceneMode.h"
#include <Objects/Scene/ImGui/ZCR_IGWCreateAndMode.h>
#include "ZCR_Mesh.h"
#include "ZCR_Camera.h"
#include "Orientation3D/ZCR_Orientation3D.h"

struct ZCR_Scene
{
    ZCR_Scene();
    ~ZCR_Scene();

    static ZCR_SceneModes GetActiveSceneMode() noexcept;
    static void SetActiveSceneMode(ZCR_SceneModes _sceneMode, bool changeFiguresState) noexcept;
    static void SetAxise(ZCR_Axis axis);

private:
    static inline ZCR_Scene* pScene;

    ZCR_IGWCreateAndMode igwCreateAndMode;
    ZCR_Camera camera {};
    ZCR_Orientation3D orientatin3D { &camera };
    ZCR_Axis axis = ZCR_A_Free;
    ZCR_Mesh mesh;
    ZCR_SceneModes activeSceneMode = ZCR_SM_Model;
    // ZC_uptr<ZCR_SceneMode> sceneModes[3];
};