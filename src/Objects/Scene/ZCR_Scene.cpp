#include "ZCR_Scene.h"

#include <Objects/Figure/ZCR_Figures.h>
#include "ZC/Video/OpenGL/Texture/ZC_Texture.h"
#include <File/ZCR_File.h>
#include <Objects/Scene/ImGui/ZCR_IconTexture.h>

ZCR_Scene::ZCR_Scene()
    : mesh(100.f)
{
    ZCR_IconTexture::iconTexture = std::move(ZC_Texture::LoadTexture2D(ZC_FSPath(ZCR_ZCRTexturePath).append("sceneIcons.png").string().c_str()));
    pScene = this;
}

ZCR_Scene::~ZCR_Scene()
{
    pScene = nullptr;
}

ZCR_SceneModes ZCR_Scene::GetActiveSceneMode() noexcept
{
    return pScene->activeSceneMode;
}

void ZCR_Scene::SetActiveSceneMode(ZCR_SceneModes _sceneMode, bool changeFiguresState) noexcept
{
    if (pScene->activeSceneMode != _sceneMode)
    {
        pScene->activeSceneMode = _sceneMode;
        if (changeFiguresState) ZCR_Figures::SwitchFiguresAndActiveFiguresToNewSceneMode(pScene->activeSceneMode);
    }
}

void ZCR_Scene::SetAxise(ZCR_Axis _axis)
{
    if (!pScene || pScene->axis == _axis) return;
    pScene->axis = _axis;
    pScene->camera.SetAxis(_axis);
}
