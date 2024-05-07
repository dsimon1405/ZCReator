#include "ZCR_Figure.h"

#include <Objects/Scene/ImGui/ZCR_IGWBM_Mode.h>

ZCR_Figure::ZCR_Figure(FigureName standartFigure)
    : name(GetName(standartFigure)),
    figureSet(GetQuads(standartFigure), GetTriangles(standartFigure), GetNormals(standartFigure))
{}

// ZCR_Figure::ZCR_Figure(ZCR_Figure&& fig)
//     : figureSet(std::move(fig.figureSet))
// {}

bool ZCR_Figure::operator == (ZCR_Figure* fig) const noexcept
{
    return this == fig;
}

void ZCR_Figure::ChangeSceneMode(ZCR_SceneMode sceneMode)
{
    figureSet.SwitchToSceneMode(sceneMode, figureSet.IsActiveOnScene());
}

void ZCR_Figure::ChangeSceneActivity(bool isActiveOnScene)
{
    if (figureSet.IsActiveOnScene() == isActiveOnScene) return;
    figureSet.SwitchToSceneMode(ZCR_IGWBM_Mode::GetActiveSceneMode(), isActiveOnScene);
}

void ZCR_Figure::ChangeSceneModeAndActivity(ZCR_SceneMode sceneMode, bool isActiveOnScene)
{
    figureSet.SwitchToSceneMode(sceneMode, isActiveOnScene);
}

void ZCR_Figure::Translate(const ZC_Vec3<float>& trans)
{
    figureSet.TranslateModelMatrix(trans);
}

std::string ZCR_Figure::GetName(FigureName standartFigure)
{
    switch (standartFigure)
    {
    case FigureName::FN_Cube: return "Cube";
    default: return "";
    }
}

ZC_sptr<ZC_DA<ZC_Quad>> ZCR_Figure::GetQuads(FigureName standartFigure)
{
    switch (standartFigure)
    {
    case FigureName::FN_Cube: return ZC_sptrMake<ZC_DA<ZC_Quad>>
        (
            new ZC_Quad[]
            {
                {
                    {-1.f, -1.f, -1.f },    //  bl
                    { 1.f, -1.f,  1.f },    //  tr
                    {-1.f, -1.f,  1.f },    //  tl
                    { 1.f, -1.f, -1.f }     //  br
                },
                {
                    {-1.f,  1.f, -1.f },
                    {-1.f, -1.f,  1.f },
                    {-1.f,  1.f,  1.f },
                    {-1.f, -1.f, -1.f }, 
                },
                {
                    { 1.f,  1.f, -1.f },
                    {-1.f,  1.f,  1.f },
                    { 1.f,  1.f,  1.f },
                    {-1.f,  1.f, -1.f }
                },
                {
                    { 1.f, -1.f, -1.f },
                    { 1.f,  1.f,  1.f },
                    { 1.f, -1.f,  1.f },
                    { 1.f,  1.f, -1.f }
                },
                {
                    {-1.f, -1.f,  1.f },
                    { 1.f,  1.f,  1.f },
                    {-1.f,  1.f,  1.f },
                    { 1.f, -1.f,  1.f }
                },
                {
                    { 1.f, -1.f, -1.f },
                    {-1.f,  1.f, -1.f },
                    { 1.f,  1.f, -1.f },
                    {-1.f, -1.f, -1.f }
                }
            },
            6
        );
    default: return nullptr;
    }
}

ZC_sptr<ZC_DA<ZC_Triangle>> ZCR_Figure::GetTriangles(FigureName standartFigure)
{
    switch (standartFigure)
    {
    case FigureName::FN_Cube: return {};
    default: return {};
    }
    // return ZC_DA<ZC_Triangle>
    // (
    //     new ZC_Triangle[]
    //     {
    //         {
    //             {-1.f, -1.f, -1.f },
    //             { 1.f, -1.f,  1.f },
    //             {-1.f, -1.f,  1.f },
    //         },
    //         {
    //             {-1.f, -1.f, -1.f },
    //             { 1.f, -1.f, -1.f },
    //             { 1.f, -1.f,  1.f },
    //         },

    //         {
    //             {-1.f,  1.f, -1.f },
    //             {-1.f, -1.f,  1.f },
    //             {-1.f,  1.f,  1.f },
    //         },
    //         {
    //             {-1.f,  1.f, -1.f },
    //             {-1.f, -1.f, -1.f },
    //             {-1.f, -1.f,  1.f },
    //         },

    //         {
    //             { 1.f, -1.f, -1.f },
    //             {-1.f,  1.f, -1.f },
    //             { 1.f,  1.f, -1.f },
    //         },
    //         {
    //             { 1.f, -1.f, -1.f },
    //             {-1.f, -1.f, -1.f },
    //             {-1.f,  1.f, -1.f },
    //         }
    //     },
    //     6
    // );
}

ZC_sptr<ZC_DA<int>> ZCR_Figure::GetNormals(FigureName standartFigure)
{
    switch (standartFigure)
    {
    case FigureName::FN_Cube: return ZC_sptrMake<ZC_DA<int>>
        (
            new int[]
            {
                524288,
                524288,
                524288,
                524288,
                
                512,
                512,
                512,
                512,

                523264,
                523264,
                523264,
                523264,

                511,
                511,
                511,
                511,
                
                535822336,
                535822336,
                535822336,
                535822336,
                
                536870912,
                536870912,
                536870912,
                536870912,
            },
            24
        );
    default: return nullptr;
    }
    
}