#include "ZCR_FigureCreator.h"

ZC_uptr<ZCR_Figure> ZCR_FigureCreator::CreateFigure(ZCR_FigureName figureName)
{
    return { new ZCR_Figure(GetName(figureName), GetQuads(figureName), GetTriangles(figureName), GetNormals(figureName)) };
}

std::string ZCR_FigureCreator::GetName(ZCR_FigureName standartFigure)
{
    switch (standartFigure)
    {
    case ZCR_FigureName::ZCR_FN_Cube: return "Cube";
    default: return "";
    }
}

ZC_DA<ZC_Quad> ZCR_FigureCreator::GetQuads(ZCR_FigureName standartFigure)
{
    switch (standartFigure)
    {
    case ZCR_FigureName::ZCR_FN_Cube: return
        {
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
        };
    default: return {};
    }
}

ZC_DA<ZC_Triangle> ZCR_FigureCreator::GetTriangles(ZCR_FigureName standartFigure)
{
    switch (standartFigure)
    {
    case ZCR_FigureName::ZCR_FN_Cube: return {};
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

ZC_DA<int> ZCR_FigureCreator::GetNormals(ZCR_FigureName standartFigure)
{
    switch (standartFigure)
    {
    case ZCR_FigureName::ZCR_FN_Cube: return
        {
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
        };
    default: return {};
    }
    
}