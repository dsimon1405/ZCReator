#pragma once

#include "ZCR_VBO.h"
#include <ZC/Video/OpenGL/Renderer/ZC_DrawerSet.h>
#include <Objects/Scene/ZCR_SceneMode.h>

class ZCR_Surface : public virtual ZCR_VBO
{
public:
    void SetAlpha(float alpha);
    float GetAlpha();
    bool IsUseLight();
    void SetUseLight(bool _useLight);

private:
    ZC_DrawerSet dsSurface;

protected:
    ZC_DSController dscSurface;

    ZCR_Surface();
    // ZCR_Surface(ZCR_Surface&& s);

    void ChangeSceneModeTriangle(ZCR_SceneMode sceneMode);

private:
    static inline float alphaOne = 1.f;
    float alpha = 1.f;
    int useLight = true;    //  uses as int in uniform

    ZC_DrawerSet CreateSurfaceDrawerSet();
    ZC_DSController MakeSurfaceRSController();
    ZC_DA<uchar> GetTriangleElements(ulong& rElementsCount, GLenum& rElementsType);
    template<typename T>
    static void FillTriangleElements(T* pElementsHead, ulong elementsSize, ulong quadsElementsCount);

};

template<typename T>
void ZCR_Surface::FillTriangleElements(T* pElementsHead, ulong elementsSize, ulong quadsElementsCount)
{
    ulong elementsI = 0,
        index = 0;
    for ( ; elementsI < quadsElementsCount; elementsI += 6)
    {
        pElementsHead[elementsI] = pElementsHead[elementsI + 3] = static_cast<T>(index++);
        pElementsHead[elementsI + 1] = pElementsHead[elementsI + 5] = static_cast<T>(index++);
        pElementsHead[elementsI + 2] = static_cast<T>(index++);
        pElementsHead[elementsI + 4] = static_cast<T>(index++);
    }
    while (elementsI < elementsSize)
    {
        pElementsHead[elementsI++] = static_cast<T>(index++);
        pElementsHead[elementsI++] = static_cast<T>(index++);
        pElementsHead[elementsI++] = static_cast<T>(index++);
    }
}