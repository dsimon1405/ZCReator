#pragma once

#include "FigureData/ZCR_Point.h"
#include "FigureData/ZCR_Line.h"
#include "FigureData/ZCR_Surface.h"
#include <Objects/Scene/ZCR_SceneMode.h>
#include <ZC/Tools/Math/ZC_Mat4.h>
#include "FigureData/ZCR_MatModel.h"

class ZCR_Figure : public ZCR_Point, public ZCR_Line, public ZCR_Surface
{
public:
    ZCR_MatModel matModel;

    ZCR_Figure(std::string&& _name, ZC_DA<ZC_Quad>&& _quads, ZC_DA<ZC_Triangle>&& _triangles, ZC_DA<int>&& _normals);
    // ZCR_Figure(ZCR_Figure&& f);

    bool operator == (ZCR_Figure* fig) const noexcept;

    void ChangeSceneMode(ZCR_SceneMode sceneMode);
    void SetSceneActivity(bool _isActiveOnScene);
    void ChangeSceneModeAndActivity(ZCR_SceneMode sceneMode, bool _isActiveOnScene);

    const char* GetName() const noexcept;
    void SetName(std::string&& _name);
    bool IsActiveOnScene() const noexcept;

private:
    static inline uint idCounter = 0;

    const uint id;
    std::string name;

    ZC_RSPDStencilBorderData* GetPointerOnStencilBorderData();
};