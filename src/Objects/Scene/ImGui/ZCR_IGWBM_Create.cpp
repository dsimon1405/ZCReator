#include "ZCR_IGWBM_Create.h"

#include <imgui.h>
#include "ZCR_IconTexture.h"
#include <Objects/Figure/ZCR_Figures.h>
#include <ZC/Events/ZC_Events.h>

ZCR_IGWBM_Create::ZCR_IGWBM_Create()
    : ZC_IGWBM(K_C, true, false, true, "Create", false, 0.f, 0.f, 0.f, 0.f, ZC_WOIF__X_Center | ZC_WOIF__Y_Center, true,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse
            | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings)
{}

void ZCR_IGWBM_Create::VDrawWindowIGW()
{
    static float createIconSize = 100.f;
    static int columns = 4;
    if (ImGui::BeginTable("ObjectCreation", columns))
    {
        static ImVec2 iconSize(createIconSize, createIconSize);
        static ImVec4 colorActive(1.f, 1.f, 1.f, 1.f),
            colorPassive(1.f, 1.f, 1.f, 0.5f);
        
        bool isAciveBM = this->IsEventsTargetBM();

        ImGui::TableNextColumn();
        static int iconTextureId = ZCR_IconTexture::iconTexture.GetId();
        if (ImGui::ImageButton("Cube", reinterpret_cast<ImTextureID>((long long)iconTextureId), iconSize, ZCR_IconTexture::tcCreateCube.tl, ZCR_IconTexture::tcCreateCube.br))
            FigureCreation(FN_Cube, false);
        ImGui::TextColored(ImGui::IsItemHovered() ? colorActive : colorPassive, isAciveBM ? "     Cube(U)" : "     Cube");
        
        ImGui::TableNextColumn();
        if (ImGui::ImageButton("Cylinder", reinterpret_cast<ImTextureID>((long long)iconTextureId), iconSize, ZCR_IconTexture::tcCreateCylinder.tl, ZCR_IconTexture::tcCreateCylinder.br))
            FigureCreation(FN_Cube, false);
        ImGui::TextColored(ImGui::IsItemHovered() ? colorActive : colorPassive, isAciveBM ? "   Cylinder(Y)" : "   Cylinder");

        ImGui::TableNextColumn();
        if (ImGui::ImageButton("Cone", reinterpret_cast<ImTextureID>((long long)iconTextureId), iconSize, ZCR_IconTexture::tcCreateCone.tl, ZCR_IconTexture::tcCreateCone.br))
            FigureCreation(FN_Cube, false);
        ImGui::TextColored(ImGui::IsItemHovered() ? colorActive : colorPassive, isAciveBM ? "     Cone(O)" : "     Cone");

        ImGui::TableNextColumn();
        if (ImGui::ImageButton("Sphere", reinterpret_cast<ImTextureID>((long long)iconTextureId), iconSize, ZCR_IconTexture::tcCreateSphere.tl, ZCR_IconTexture::tcCreateSphere.br))
            FigureCreation(FN_Cube, false);
        ImGui::TextColored(ImGui::IsItemHovered() ? colorActive : colorPassive, isAciveBM ? "    Sphere(S)" : "    Sphere");

        ImGui::EndTable();
    }
}

void ZCR_IGWBM_Create::VCallButtonDownBM(ZC_ButtonID buttonId, float time)
{
    switch (buttonId)
    {
    case K_U: FigureCreation(FN_Cube, true); break;
    default: break;
    }
}

void ZCR_IGWBM_Create::FigureCreation(int _creatingFigureName, bool createNow)
{
    NeedDrawIGW(false);
    if (createNow) ZCR_Figures::CreateFigure(static_cast<FigureName>(_creatingFigureName));
    else
    {
        creatingFigureName = _creatingFigureName;
        this->AddAfterDrawEvent({ &ZCR_IGWBM_Create::CreateFigure, this });
    }
}

void ZCR_IGWBM_Create::CreateFigure()
{
    ZCR_Figures::CreateFigure(static_cast<FigureName>(creatingFigureName));
}