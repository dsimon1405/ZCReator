#pragma once

#include "ZCR_SceneMode.h"
#include <Objects/Scene/ImGui/ZCR_IGWCreateAndMode.h>
#include "ZCR_Mesh.h"
#include "ZCR_Camera.h"
#include <Objects/Scene/ZCR_Orientation3D.h>

struct ZCR_Scene
{
    typedef typename ZCR_SceneMode::Mode SceneMode;

    ZCR_Scene();

    static SceneMode GetActiveSceneMode() noexcept;
    static void SetActiveSceneMode(SceneMode _sceneMode, bool changeFiguresState) noexcept;

private:
    static inline ZCR_Scene* pScene;

    ZCR_IGWCreateAndMode igwCreateAndMode;
    ZCR_Camera camera {};
    ZCR_Mesh mesh;
    SceneMode activeSceneMode = SceneMode::Model;
    ZC_uptr<ZCR_SceneMode> sceneModes[3];
};