#include "ZCR_IGWBM_Interface.h"

#include <Objects/Scene/ZCR_Scene.h>
#include <ZC/Objects/Camera/ZC_Camera.h>
#include <ZC_IGWindow.h>
#include <imgui.h>

ZCR_IGWBM_Interface::ZCR_IGWBM_Interface()
    : ZC_IGWBM(K_I, true, false, true, "Interface", false, 400.f, 300.f, 0.f, 0.f, ZC_WOIF__X_Center | ZC_WOIF__Y_Center, true,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse
        | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings)
{}

void ZCR_IGWBM_Interface::VCallButtonDownBM(ZC_ButtonID buttonId, float time)
{
    switch (buttonId)
    {
    case K_O: orientatin3D.IsActive() ? orientatin3D.Deactivate() : orientatin3D.Activate(); break;
    case K_G: ZC_IGWindow::NeedImGuiDraw(!ZC_IGWindow::IsImGuiDrawing()); break;
    case K_1: ZCR_Scene::GetScene()->SetMeshSpecificCoordSystem(ZCR_Mesh::CS_Auto); break;
    case K_2: ZCR_Scene::GetScene()->SetMeshSpecificCoordSystem(ZCR_Mesh::CS_XY); break;
    case K_3: ZCR_Scene::GetScene()->SetMeshSpecificCoordSystem(ZCR_Mesh::CS_XZ); break;
    case K_4: ZCR_Scene::GetScene()->SetMeshSpecificCoordSystem(ZCR_Mesh::CS_YZ); break;
    case K_5: ZCR_Scene::GetScene()->SetMeshSpecificCoordSystem(ZCR_Mesh::CS_None); break;
    default: break;
    }
}

void ZCR_IGWBM_Interface::VDrawWindowIGW()
{
    bool isActiveBM = this->IsEventsTargetBM();
    DrawInterface(isActiveBM);
    DrawMeshOptions(isActiveBM);
    DrawViewportOptions();
}

void ZCR_IGWBM_Interface::DrawInterface(bool isActiveBM)
{
    ImGui::SeparatorText("Interface");
    bool isActiveO3d = orientatin3D.IsActive();
    if (ImGui::Checkbox(isActiveBM ? "Orientation3D(O)" : "Orientation3D", &isActiveO3d)) isActiveO3d ? orientatin3D.Activate() : orientatin3D.Deactivate();
    bool isDrawingIG = ZC_IGWindow::IsImGuiDrawing();
    if (ImGui::Checkbox(isActiveBM ? "ImGui(G)" : "ImGui", &isDrawingIG)) ZC_IGWindow::NeedImGuiDraw(!isDrawingIG);
}

void ZCR_IGWBM_Interface::DrawMeshOptions(bool isActiveBM)
{
    ImGui::SeparatorText("Mesh");
    if (ImGui::BeginTable("Mesh", 5))
    {
        ZCR_Scene* pScene = ZCR_Scene::GetScene();
        ImGui::TableNextColumn();
        if (ImGui::RadioButton(isActiveBM ? "Auto(1)" : "Auto", pScene->GetMeshSpecificCoordSystem() == ZCR_Mesh::CS_Auto)) pScene->SetMeshSpecificCoordSystem(ZCR_Mesh::CS_Auto);
        ImGui::TableNextColumn();
        if (ImGui::RadioButton(isActiveBM ? "XY(2)" : "XY", pScene->GetMeshSpecificCoordSystem() == ZCR_Mesh::CS_XY)) pScene->SetMeshSpecificCoordSystem(ZCR_Mesh::CS_XY);
        ImGui::TableNextColumn();
        if (ImGui::RadioButton(isActiveBM ? "XZ(3)" : "XZ", pScene->GetMeshSpecificCoordSystem() == ZCR_Mesh::CS_XZ)) pScene->SetMeshSpecificCoordSystem(ZCR_Mesh::CS_XZ);
        ImGui::TableNextColumn();
        if (ImGui::RadioButton(isActiveBM ? "YZ(4)" : "YZ", pScene->GetMeshSpecificCoordSystem() == ZCR_Mesh::CS_YZ)) pScene->SetMeshSpecificCoordSystem(ZCR_Mesh::CS_YZ);
        ImGui::TableNextColumn();
        if (ImGui::RadioButton(isActiveBM ? "None(5)" : "None", pScene->GetMeshSpecificCoordSystem() == ZCR_Mesh::CS_None)) pScene->SetMeshSpecificCoordSystem(ZCR_Mesh::CS_None);
        
        ImGui::EndTable();
    }
}

void ZCR_IGWBM_Interface::DrawViewportOptions()
{
    ZC_Camera* pCamera = ZC_Camera::GetActiveCamera();
    float nearPlane = pCamera->GetNearPlane();
    float farPlane = pCamera->GetFarPlane();
    float fovy = pCamera->GetFovy();

    ImGui::SeparatorText("Viewport");
    if (ImGui::BeginTable("Viewport", 2))
    {
        ImGui::TableSetupColumn("1", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("2", ImGuiTableColumnFlags_WidthFixed);
        
        static const float nearPlaneDefault = 0.01f;
        ImGui::TableNextColumn();
        if (ImGui::InputFloat("NearPlane", &nearPlane, 0.1f, 2.f, "%.2f"))
        {
            if (nearPlane >= farPlane) nearPlane = farPlane - 1.f;
            if (nearPlane <= 0.f) nearPlane = nearPlaneDefault;
            pCamera->SetNearPlane(nearPlane);
        }
        ImGui::TableNextColumn();
        if (nearPlane != nearPlaneDefault && ImGui::SmallButton("Default##1")) pCamera->SetNearPlane(nearPlaneDefault);

        static const float farPlaneDefault = 100.f;
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        if (ImGui::InputFloat("FarPlane", &farPlane, 1.f, 5.f, "%.2f"))
        {
            static const float farPlaneMax = 2000.f;
            if (nearPlane >= farPlane) farPlane = nearPlane + 1.f;
            else if (farPlane > farPlaneMax) farPlane = farPlaneMax;
            pCamera->SetFarPlane(farPlane);
        }
        ImGui::TableNextColumn();
        if (farPlane != farPlaneDefault && ImGui::SmallButton("Default##2")) pCamera->SetFarPlane(farPlaneDefault);

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        if (ImGui::SliderFloat("Fovy", &fovy, 1.f, 179.f, "%1.f")) pCamera->SetFovy(fovy);
        ImGui::TableNextColumn();
        static const float fovyDefault = 45.f;
        if (fovy != fovyDefault && ImGui::SmallButton("Default##3")) pCamera->SetFovy(fovyDefault);

        ImGui::EndTable();
    }
}