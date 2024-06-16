#pragma once

#include <File/ZCR_File.h>

enum ZCR_ShVertex
{
    ZCR_SV_None = -1,

    ZCR_SV_colorFigure,
    ZCR_SV_point,
    ZCR_SV_lineFigure,
    ZCR_SV_stencilBorder,
    ZCR_SV_texture_Vertex_TexCoord,
    ZCR_SV_lineMesh,
    ZCR_SV_textWindow,
    ZCR_SV_textScene,
    ZCR_SV_textWindowIntoScene,

    ZCR_SV_All
};

enum ZCR_ShTessControl
{
    ZCR_STC_None = -1,

    ZCR_STC_All
};

enum ZCR_ShTessEvaluation
{
    ZCR_STE_None = -1,

    ZCR_STE_All
};

enum ZCR_ShGeometry
{
    ZCR_SG_None = -1,

    ZCR_SG_All
};

enum ZCR_ShFragment
{
    ZCR_SF_None = -1,
    
    ZCR_SF_color,
    ZCR_SF_colorTex,
    ZCR_SF_text,
    ZCR_SF_orthoTexture,
    
    ZCR_SF_All
};


class ZCR_ShaderLoader  //  class for loading all shaders
{
public:
    ZCR_ShaderLoader() = delete;
    
    static bool LoadAllShaders();

private:
    static std::string GetPath_Vertex(int shV, const ZC_FSPath& shadersPath);
    static std::string GetPath_TessControl(int shTC, const ZC_FSPath& shadersPath);
    static std::string GetPath_TessEvaluation(int shTE, const ZC_FSPath& shadersPath);
    static std::string GetPath_Geometry(int shG, const ZC_FSPath& shadersPath);
    static std::string GetPath_Fragment(int shF, const ZC_FSPath& shadersPath);
};