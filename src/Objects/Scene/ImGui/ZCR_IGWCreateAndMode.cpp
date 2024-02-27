#include "ZCR_IGWCreateAndMode.h"

#include <imgui.h>
#include <Objects/Scene/ZCR_Scene.h>

ZCR_IGWCreateAndMode::ZCR_IGWCreateAndMode()
    : ZC_IGWindow("CreateAndMode", true, 130.f, 100.f, 0.f, 0.f, WindowIndentFlags::XLeft_YTop, false,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration)
{}
#include <ZC/Tools/Console/ZC_cout.h>
void ZCR_IGWCreateAndMode::DrawWindow()
{
    // ImGui::ShowDemoWindow();
    if (ImGui::Button("Create Figure", { 130.f, 20.f })) wFigures.NeedDraw(true);

    typedef typename ZCR_SceneMode::Mode SceneMode;
    if (ImGui::RadioButton("Model", ZCR_Scene::GetActiveSceneMode() == SceneMode::Model)) ZCR_Scene::SetActiveSceneMode(SceneMode::Model, true);
    if (ImGui::RadioButton("Edit", ZCR_Scene::GetActiveSceneMode() == SceneMode::Edit)) ZCR_Scene::SetActiveSceneMode(SceneMode::Edit, true);
    if (ImGui::RadioButton("Sculpting", ZCR_Scene::GetActiveSceneMode() == SceneMode::Sculpting)) ZCR_Scene::SetActiveSceneMode(SceneMode::Sculpting, true);
}
