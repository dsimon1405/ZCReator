#pragma once

#include <ZC_ImGui.h>
#include <ZC/Video/OpenGL/Texture/ZC_Textures.h>

class ZCR_ImGui : public ZC_ImGui
{
public:
    ZC_Texture image = ZC_Textures::LoadTexture2D("/home/dmitry/Загрузки/awesomeface.png");

    void ImGuiDraw() override;

private:
    void MainWindow();

    /*
    If currentState - true, than sets state needOpen and change currentState on false. If currentState - false nothing happens.
    If needOpen - true, than CollapsingHeader gonna be open, otherwise close.
    */
    void SetOpenOrCloseStateOfCollapsingHeader(bool& rCurrentState, bool needOpen) noexcept;
};