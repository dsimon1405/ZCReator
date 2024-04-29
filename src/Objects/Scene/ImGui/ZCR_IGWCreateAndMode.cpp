#include "ZCR_IGWCreateAndMode.h"

#include <imgui.h>
#include <Objects/Scene/ZCR_Scene.h>

ZCR_IGWCreateAndMode::ZCR_IGWCreateAndMode()
    : ZC_IGWindow("CreateAndMode", true, 130.f, 120.f, 10.f, 30.f, ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Top_Pixel, false,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse)
{}

void ZCR_IGWCreateAndMode::DrawWindow()
{
    // ImGui::ShowDemoWindow();
    if (ImGui::Button("Scene", { 100.f, 20.f })) igwScene.NeedDraw(true);
    if (ImGui::Button("Create Figure", { 100.f, 20.f })) igwFigures.NeedDraw(true);
    
    ZCR_Scene* pScene = ZCR_Scene::GetScene();
    if (ImGui::RadioButton("Model", pScene->GetActiveSceneMode() == ZCR_SM_Model)) pScene->SetActiveSceneMode(ZCR_SM_Model, true);
    if (ImGui::RadioButton("Edit", pScene->GetActiveSceneMode() == ZCR_SM_Edit)) pScene->SetActiveSceneMode(ZCR_SM_Edit, true);
    if (ImGui::RadioButton("Sculpting", pScene->GetActiveSceneMode() == ZCR_SM_Sculpting)) pScene->SetActiveSceneMode(ZCR_SM_Sculpting, true);
}
