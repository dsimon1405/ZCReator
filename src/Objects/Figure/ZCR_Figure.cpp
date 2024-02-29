#include "ZCR_Figure.h"

ZCR_Figure::ZCR_Figure(Name standartFigure)
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

void ZCR_Figure::ChangeSceneMode(SceneMode sceneMode)
{
    figureSet.SwitchToSceneMode(sceneMode, isActiveOnScene);
}

void ZCR_Figure::ChangeSceneActivity(bool _isActiveOnScene)
{
    if (isActiveOnScene == _isActiveOnScene) return;
    isActiveOnScene = _isActiveOnScene;
    figureSet.SwitchToSceneMode(ZCR_Scene::GetActiveSceneMode(), isActiveOnScene);
}

void ZCR_Figure::ChangeSceneModeAndActivity(SceneMode sceneMode, bool _isActiveOnScene)
{
    isActiveOnScene = _isActiveOnScene;
    figureSet.SwitchToSceneMode(sceneMode, isActiveOnScene);
}

void ZCR_Figure::Translate(const ZC_Vec3<float>& trans)
{
    figureSet.TranslateModelMatrix(trans);
}

std::string ZCR_Figure::GetName(Name standartFigure)
{
    switch (standartFigure)
    {
    case Name::Cube: return "Cube";
    default: return "";
    }
}

ZC_sptr<ZC_DA<ZC_Quad>> ZCR_Figure::GetQuads(Name standartFigure)
{
    switch (standartFigure)
    {
    case Name::Cube: return ZC_sptrMake<ZC_DA<ZC_Quad>>
        (
            new ZC_Quad[]
            {
                {
                    {-1.f, -1.f, -1.f },
                    { 1.f, -1.f,  1.f },
                    {-1.f, -1.f,  1.f },
                    { 1.f, -1.f, -1.f }
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

ZC_sptr<ZC_DA<ZC_Triangle>> ZCR_Figure::GetTriangles(Name standartFigure)
{
    switch (standartFigure)
    {
    case Name::Cube: return {};
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

ZC_sptr<ZC_DA<int>> ZCR_Figure::GetNormals(Name standartFigure)
{
    switch (standartFigure)
    {
    case Name::Cube: return ZC_sptrMake<ZC_DA<int>>
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