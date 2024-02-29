#include "ZCR_IGWFigures.h"

#include <imgui.h>
#include "ZCR_IconTexture.h"
#include <Objects/Figure/ZCR_Figures.h>
#include <ZC/Events/ZC_Events.h>

ZCR_IGWFigures::ZCR_IGWFigures()
    : ZC_IGWindow("Figures", false, 465.f, 160.f, 0.f, 0.f, WindowIndentFlags::Center, true,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)
{}

void ZCR_IGWFigures::DrawWindow()
{
    static float createIconSize = 100.f;
    static float columns = 4.f;
    if (ImGui::BeginTable("FiguresTable", columns, 0, ImVec2(450.f, 0)))
    {
        static ImVec2 iconSize(createIconSize, createIconSize);
        static ImVec4 colorActive(1.f, 1.f, 1.f, 1.f),
            colorPassive(1.f, 1.f, 1.f, 0.5f);
        
        ImGui::TableNextColumn();
        if (ImGui::ImageButton("Cube", reinterpret_cast<ImTextureID>(ZCR_IconTexture::iconTexture.GetId()),
                iconSize, ZCR_IconTexture::tcCreateCube.tl, ZCR_IconTexture::tcCreateCube.br))
            CreateFigure(ZCR_Figure::Name::Cube);
        ImGui::IsItemHovered() ? ImGui::TextColored(colorActive, "      Cube") : ImGui::TextColored(colorPassive, "      Cube");
        
        ImGui::TableNextColumn();
        if (ImGui::ImageButton("Cylinder", reinterpret_cast<ImTextureID>(ZCR_IconTexture::iconTexture.GetId()),
                iconSize, ZCR_IconTexture::tcCreateCylinder.tl, ZCR_IconTexture::tcCreateCylinder.br))
            CreateFigure(ZCR_Figure::Name::Cube);
        ImGui::IsItemHovered() ? ImGui::TextColored(colorActive, "    Cylinder") : ImGui::TextColored(colorPassive, "    Cylinder");

        ImGui::TableNextColumn();
        if (ImGui::ImageButton("Cone", reinterpret_cast<ImTextureID>(ZCR_IconTexture::iconTexture.GetId()),
                iconSize, ZCR_IconTexture::tcCreateCone.tl, ZCR_IconTexture::tcCreateCone.br))
            CreateFigure(ZCR_Figure::Name::Cube);
        ImGui::IsItemHovered() ? ImGui::TextColored(colorActive, "      Cone") : ImGui::TextColored(colorPassive, "      Cone");

        ImGui::TableNextColumn();
        if (ImGui::ImageButton("Sphere", reinterpret_cast<ImTextureID>(ZCR_IconTexture::iconTexture.GetId()),
                iconSize, ZCR_IconTexture::tcCreateSphere.tl, ZCR_IconTexture::tcCreateSphere.br))
            CreateFigure(ZCR_Figure::Name::Cube);
        ImGui::IsItemHovered() ? ImGui::TextColored(colorActive, "     Sphere") : ImGui::TextColored(colorPassive, "     Sphere");

        ImGui::EndTable();
    }
}

void ZCR_IGWFigures::CreateFigure(int _creatingFigureName)
{
    NeedDraw(false);
    creatingFigureName = _creatingFigureName;
    sconHandleEventsEnd = ZC_Events::ConnectHandleEventsEnd({ &ZCR_IGWFigures::Callback, this });
}

void ZCR_IGWFigures::Callback()
{
    ZCR_Figures::CreateFigure(static_cast<typename ZCR_Figure::Name>(creatingFigureName));
    sconHandleEventsEnd.Disconnect();
}