#include "ZCR_IGWScene.h"

#include <Objects/Scene/ZCR_Scene.h>

#include <imgui.h>

ZCR_IGWScene::ZCR_IGWScene()
    : ZC_IGWBM(K_I, true, false, "Scene", false, 300.f, 160.f, 0.f, 0.f, ZC_WOIF__X_Center | ZC_WOIF__Y_Center, true,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)
{}

void ZCR_IGWScene::VDrawWindowIGW()
{
    ImGui::Text("                   Mesh");
    if (ImGui::BeginTable("Mesh", 5, 0))
    {
        ZCR_Scene* pScene = ZCR_Scene::GetScene();
        ImGui::TableNextColumn();
        if (ImGui::RadioButton("Auto", pScene->GetMeshSpecificCoordSystem() == ZCR_Mesh::CS_Auto)) pScene->SetMeshSpecificCoordSystem(ZCR_Mesh::CS_Auto);
        ImGui::TableNextColumn();
        if (ImGui::RadioButton("XY", pScene->GetMeshSpecificCoordSystem() == ZCR_Mesh::CS_XY)) pScene->SetMeshSpecificCoordSystem(ZCR_Mesh::CS_XY);
        ImGui::TableNextColumn();
        if (ImGui::RadioButton("XZ", pScene->GetMeshSpecificCoordSystem() == ZCR_Mesh::CS_XZ)) pScene->SetMeshSpecificCoordSystem(ZCR_Mesh::CS_XZ);
        ImGui::TableNextColumn();
        if (ImGui::RadioButton("YZ", pScene->GetMeshSpecificCoordSystem() == ZCR_Mesh::CS_YZ)) pScene->SetMeshSpecificCoordSystem(ZCR_Mesh::CS_YZ);
        ImGui::TableNextColumn();
        if (ImGui::RadioButton("None", pScene->GetMeshSpecificCoordSystem() == ZCR_Mesh::CS_None)) pScene->SetMeshSpecificCoordSystem(ZCR_Mesh::CS_None);

        ImGui::EndTable();
    }

    if (ImGui::Checkbox("Orientation3D", &checkBoxIsOrienttion3D)) checkBoxIsOrienttion3D ? orientatin3D.Activate() : orientatin3D.Deactivate();
}

void ZCR_IGWScene::VCallButtonDownBM(ZC_ButtonID buttonId, float time)
{
    switch (buttonId)
    {
    case K_O:
    {
        checkBoxIsOrienttion3D = !checkBoxIsOrienttion3D;
        checkBoxIsOrienttion3D ? orientatin3D.Activate() : orientatin3D.Deactivate();
    } break;
    
    default: break;
    }
}