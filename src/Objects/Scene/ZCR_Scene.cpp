#include "ZCR_Scene.h"

#include "ZC/Video/OpenGL/Texture/ZC_Texture.h"
#include <File/ZCR_File.h>
#include <Objects/Scene/ImGui/ZCR_IconTexture.h>
#include <ZC/Video/ZC_SWindow.h>

ZCR_Scene::ZCR_Scene()
    : ZCR_Mesh(100.f)
{
    ZCR_IconTexture::iconTexture = std::move(ZC_Texture::LoadTexture2D(ZC_FSPath(ZCR_ZCRTexturePath).append("sceneIcons.png").string().c_str(), 0));
    pScene = this;

    ZC_SWindow::GlClearColor(0.3f, 0.3f, 0.3f, 1.f);
    ZC_SWindow::GlEnablePointSize();
}

ZCR_Scene::~ZCR_Scene()
{
    pScene = nullptr;
}

ZCR_Scene* ZCR_Scene::GetScene() noexcept
{
    return pScene;
}

void ZCR_Scene::SetAxise(ZCR_Axis _axis)
{
    if (!pScene || pScene->axis == _axis) return;
    axis = _axis;
    this->SetCameraAxis(pScene->axis);
    this->SetMeshAxis(pScene->axis);
}

ZCR_Axis ZCR_Scene::GetAxis() const noexcept
{
    return axis;
}