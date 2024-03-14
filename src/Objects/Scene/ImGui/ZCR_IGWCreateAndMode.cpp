#include "ZCR_IGWCreateAndMode.h"

#include <imgui.h>
#include <Objects/Scene/ZCR_Scene.h>

ZCR_IGWCreateAndMode::ZCR_IGWCreateAndMode()
    : ZC_IGWindow("CreateAndMode", true, 130.f, 100.f, 10.f, 30.f, ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Top_Pixel, false,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration)
{}

void ZCR_IGWCreateAndMode::DrawWindow()
{
    // ImGui::ShowDemoWindow();
    if (ImGui::Button("Create Figure", { 130.f, 20.f })) winFigures.NeedDraw(true);

    typedef typename ZCR_SceneMode::Mode SceneMode;
    if (ImGui::RadioButton("Model", ZCR_Scene::GetActiveSceneMode() == SceneMode::Model)) ZCR_Scene::SetActiveSceneMode(SceneMode::Model, true);
    if (ImGui::RadioButton("Edit", ZCR_Scene::GetActiveSceneMode() == SceneMode::Edit)) ZCR_Scene::SetActiveSceneMode(SceneMode::Edit, true);
    if (ImGui::RadioButton("Sculpting", ZCR_Scene::GetActiveSceneMode() == SceneMode::Sculpting)) ZCR_Scene::SetActiveSceneMode(SceneMode::Sculpting, true);
}
