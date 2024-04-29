#pragma once

#include <ZC_IGWindow.h>
#include <ZC/Tools/Signal/ZC_SConnection.h>
#include <Objects/Scene/Orientation3D/ZCR_Orientation3D.h>
#include <ZC/Events/ZC_SingleClickButton.h>

class ZCR_IGWScene : public ZC_IGWindow
{
public:
    ZCR_IGWScene();

private:
    ZCR_Orientation3D orientatin3D;

    void DrawWindow() override;
};