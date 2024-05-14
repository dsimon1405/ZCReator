#include "ZCR_IGW_Figures.h"

#include <Objects/Figure/ZCR_Figures.h>

#include <imgui.h>

ZCR_IGW_Figures::ZCR_IGW_Figures(std::list<ZC_uptr<ZCR_Figure>>& _figures)
    : ZC_IGWindow("Figure", true, 170.f, 400.f, 0.f, 0.f, ZC_WOIF__X_Right_Pixel | ZC_WOIF__Y_Center, false, 
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse
        | ImGuiWindowFlags_AlwaysAutoResize
        | ImGuiWindowFlags_NoSavedSettings),
    figures(_figures)
{}

void ZCR_IGW_Figures::VDrawWindowIGW()
{
    ZCR_Figure* pActiveFigure = ZCR_Figures::GetActiveFigure();
    DrawFigures(pActiveFigure);
    if (!pActiveFigure) return;   //  no active figures

    DrawTRS(1, 1.f, "Translate", pActiveFigure->matModel, &ZCR_MatModel::GetTranslate, &ZCR_MatModel::SetTranslate);
    DrawTRS(2, 1.f, "Rotate", pActiveFigure->matModel, &ZCR_MatModel::GetRotateAngle, &ZCR_MatModel::SetRotateAngle);
    DrawTRS(3, 0.1f, "Scale", pActiveFigure->matModel, &ZCR_MatModel::GetScale, &ZCR_MatModel::SetScale);
    DrawAdditional(pActiveFigure);
}

void ZCR_IGW_Figures::DrawFigures(ZCR_Figure* pActiveFigure)
{
    if (ImGui::BeginListBox("##Figures"))
    {
        for (auto& upFigure : figures)
        {
            if (ImGui::Selectable(upFigure->GetName(), upFigure == pActiveFigure))
            {
                ZCR_Figures::MakeFigureActive(upFigure.Get());
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndListBox();
    }
};

void ZCR_IGW_Figures::DrawTRS(char buttonId, float step, const char* name, ZCR_MatModel& rMatModel, float(ZCR_MatModel::*pFuncGet)(ZCR_MatModelAxis),
    void(ZCR_MatModel::* pFuncSet)(ZCR_MatModelAxis, float, bool))
{
    ImGui::SeparatorText(name);
    float x = (rMatModel.*pFuncGet)(ZCR_MMA_X);
    if (ImGui::DragFloat(buttonId == 1 ? " X##1" : buttonId == 2 ? " X##2" : " X##3", &x, step, 0.f, 0.f, "%.2f")) (rMatModel.*pFuncSet)(ZCR_MMA_X, x, true);
    float y = (rMatModel.*pFuncGet)(ZCR_MMA_Y);
    if (ImGui::DragFloat(buttonId == 1 ? " Y##1" : buttonId == 2 ? " Y##2" : " Y##3", &y, step, 0.f, 0.f, "%.2f")) (rMatModel.*pFuncSet)(ZCR_MMA_Y, y, true);
    float z = (rMatModel.*pFuncGet)(ZCR_MMA_Z);
    if (ImGui::DragFloat(buttonId == 1 ? " Z##1" : buttonId == 2 ? " Z##2" : " Z##3", &z, step, 0.f, 0.f, "%.2f")) (rMatModel.*pFuncSet)(ZCR_MMA_Z, z, true);
}

void ZCR_IGW_Figures::DrawAdditional(ZCR_Figure* pFigure)
{
    ImGui::SeparatorText("Additional");

    bool useLight = pFigure->IsUseLight();
    if (ImGui::Checkbox(" Light", &useLight)) pFigure->SetUseLight(useLight);

    bool isDrawLines = pFigure->IsDrawLines();
    if (ImGui::Checkbox(" Lines", &isDrawLines)) pFigure->SetDrawLines(isDrawLines);

    float alpha = pFigure->GetAlpha();
    if (ImGui::DragFloat(" Alpha", &alpha, 0.01, 0.f, 1.f, "%.2f")) pFigure->SetAlpha(alpha);
}