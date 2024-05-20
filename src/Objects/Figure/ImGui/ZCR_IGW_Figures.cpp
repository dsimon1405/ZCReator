#include "ZCR_IGW_Figures.h"

#include <Objects/Figure/ZCR_Figures.h>

#include <imgui.h>

ZCR_IGW_Figures::ZCR_IGW_Figures(std::list<ZC_uptr<ZCR_Figure>>& _figures)
    : ZC_IGWindow("Figures", true, 170.f, 400.f, 0.f, 0.f, ZC_WOIF__X_Right_Pixel | ZC_WOIF__Y_Center, false, 
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse
        | ImGuiWindowFlags_AlwaysAutoResize
        | ImGuiWindowFlags_NoSavedSettings),
    ZC_IGInputText(" Name", 50),
    rFigures(_figures)
{}

void ZCR_IGW_Figures::SetNameBuffer(ZCR_Figure* pFigure)
{
    inputTextBuffer.Fill('\0');    //  clear buffer
    pFigure->GetName().copy(inputTextBuffer.pHead, pFigure->GetName().find_last_of('#') - 1);
}

void ZCR_IGW_Figures::VDrawWindowIGW()
{
    // auto q = ImGui::GetWindowSize();

    ZCR_Figure* pActiveFigure = ZCR_Figures::GetActiveFigure();
    DrawFigures(pActiveFigure);
    if (!pActiveFigure) return;   //  no active figures

    ImGui::SeparatorText("Active Figure");
    DrawIGIT();
    DrawScapeTransfomations(pActiveFigure);
    DrawAdditional(pActiveFigure);
}

#include <iostream>
// void ZCR_IGW_Figures::VInputCallbackIGIT(ZC_DA<char>& buffer)
// {
//     NameChangeBufferEvent(buffer);
//     std::cout<<"enter"<<std::endl;
// }

void ZCR_IGW_Figures::VLostFocuseIGIT(ZC_DA<char>& buffer)
{
    NameChangeBufferEvent(buffer);
    std::cout<<"lost focus"<<std::endl;
}

void ZCR_IGW_Figures::DrawFigures(ZCR_Figure* pActiveFigure)
{
    if (ImGui::BeginListBox("##Figures"))
    {
        for (auto& upFigure : rFigures)
        {
            if (ImGui::Selectable(upFigure->GetName().c_str(), upFigure == pActiveFigure))
            {
                ZCR_Figures::MakeFigureActive(upFigure.Get());
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndListBox();
    }
};

void ZCR_IGW_Figures::NameChangeBufferEvent(ZC_DA<char>& buffer)
{
    int symbolsCount = buffer.size - 1;  //  count of symbols in mame (initializes as index of - buffer)
    for ( ; symbolsCount > -1; --symbolsCount)     //  is first symbol, not a whitespace from back (make from index of buffer, count of symbols)
        if (buffer[symbolsCount] != ' ' && buffer[symbolsCount] != '\0'){ ++symbolsCount; break; }

    ZCR_Figure* pActiveFigure = ZCR_Figures::GetActiveFigure();
    if (symbolsCount == 0) SetNameBuffer(pActiveFigure);  //  no symbols in buffer, set current name
    else
    {
        std::string newName(buffer.pHead, symbolsCount);
        newName.find("##") == std::string::npos ? pActiveFigure->SetName(std::move(newName)) : SetNameBuffer(pActiveFigure);
    }
}

void ZCR_IGW_Figures::DrawScapeTransfomations(ZCR_Figure* pActiveFigure)
{
    //  Draw translate, rotate, scael. buttonId - specifies different id for different buttons with same name.
    auto lambDrawTRS = [](ZC_IGTextCentered& textCentered, char buttonId, float step, ZCR_MatModel& rMatModel, float(ZCR_MatModel::*pFuncGet)(ZCR_MatModelAxis),
        void(ZCR_MatModel::* pFuncSet)(ZCR_MatModelAxis, float, bool))
    {
        textCentered.Draw();

        float x = (rMatModel.*pFuncGet)(ZCR_MMA_X);
        if (ImGui::DragFloat(buttonId == 1 ? " X##1" : buttonId == 2 ? " X##2" : " X##3", &x, step, 0.f, 0.f, "%.2f")) (rMatModel.*pFuncSet)(ZCR_MMA_X, x, true);
        float y = (rMatModel.*pFuncGet)(ZCR_MMA_Y);
        if (ImGui::DragFloat(buttonId == 1 ? " Y##1" : buttonId == 2 ? " Y##2" : " Y##3", &y, step, 0.f, 0.f, "%.2f")) (rMatModel.*pFuncSet)(ZCR_MMA_Y, y, true);
        float z = (rMatModel.*pFuncGet)(ZCR_MMA_Z);
        if (ImGui::DragFloat(buttonId == 1 ? " Z##1" : buttonId == 2 ? " Z##2" : " Z##3", &z, step, 0.f, 0.f, "%.2f")) (rMatModel.*pFuncSet)(ZCR_MMA_Z, z, true);
    };

    static ZC_IGTextCentered tcTranslate("\nTranslate"),
        tcRotate("\nRotate"),
        tcScale("\nScale");
    lambDrawTRS(tcTranslate, 1, 1.f, pActiveFigure->matModel, &ZCR_MatModel::GetTranslate, &ZCR_MatModel::SetTranslate);
    lambDrawTRS(tcRotate, 2, 1.f, pActiveFigure->matModel, &ZCR_MatModel::GetRotateAngle, &ZCR_MatModel::SetRotateAngle);
    lambDrawTRS(tcScale, 3, 0.1f, pActiveFigure->matModel, &ZCR_MatModel::GetScale, &ZCR_MatModel::SetScale);
}

void ZCR_IGW_Figures::DrawAdditional(ZCR_Figure* pFigure)
{
    static ZC_IGTextCentered tcTranslate("\nAdditional");
    tcTranslate.Draw();

    bool useLight = pFigure->IsUseLight();
    if (ImGui::Checkbox(" Light", &useLight)) pFigure->SetUseLight(useLight);

    bool isDrawLines = pFigure->IsDrawLines();
    if (ImGui::Checkbox(" Lines", &isDrawLines)) pFigure->SetDrawLines(isDrawLines);

    float alpha = pFigure->GetAlpha();
    if (ImGui::DragFloat(" Alpha", &alpha, 0.01, 0.f, 1.f, "%.2f")) pFigure->SetAlpha(alpha);
}