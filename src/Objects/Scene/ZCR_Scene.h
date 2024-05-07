#pragma once

#include <Objects/Scene/ImGui/ZCR_IGWBM_MainMenu.h>
#include "ZCR_Mesh.h"
#include "ZCR_Camera.h"

class ZCR_Scene : public ZCR_Mesh, public ZCR_Camera
{
public:
    ZCR_Scene();
    ~ZCR_Scene();

    static ZCR_Scene* GetScene() noexcept;

    void SetAxise(ZCR_Axis axis);
    ZCR_Axis GetAxis() const noexcept;

private:
    static inline ZCR_Scene* pScene;

    ZCR_IGWBM_MainMenu igwbmMenu;
    ZCR_Axis axis = ZCR_A_Free;
};