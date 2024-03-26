#include <ZC/main/ZC_main.h>
#include <ZC/Video/ZC_Window.h>

#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>
#include <ZC/Video/OpenGL/VAO/ZC_VAOs.h>
#include <ZC/Video/OpenGL/VAO/ZC_VAOConfig.h>
#include <ZC/Video/OpenGL/Buffer/ZC_UBOs.h>
#include <ZC/Video/OpenGL/Buffer/ZC_Buffers.h>
#include <ZC/Objects/Camera/ZC_Camera.h>

#include <ZC/Tools/Time/ZC_Clock.h>

#include <ZC/Tools/Math/ZC_Mat.h>
#include <ZC/Tools/Console/ZC_cout.h>

#include <ZC/Events/ZC_Events.h>

#include <ZC/Objects/ZC_Object.h>

#include <ZC/Tools/Math/ZC_Figures.h>

#include <ZC/Video/OpenGL/Texture/ZC_Textures.h>

// #include <Objects/ImGui/ZCR_ImGui.h>
#include <Objects/Scene/ZCR_Scene.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>


struct NewSquare
{
    ZC_Quad bl;
    ZC_Quad tl;
    ZC_Quad tr;
    ZC_Quad br;
    void SetCenter(const ZC_Vec3<float>& center) { bl.tr = tl.br = tr.bl = br.tl = center; }
    ZC_Vec3<float>& GetCenter() { return bl.tr; }
    void SetEdgePointL(const ZC_Vec3<float>& point) { bl.tl = tl.bl = point; }
    void SetEdgePointT(const ZC_Vec3<float>& point) { tl.tr = tr.tl = point; }
    void SetEdgePointR(const ZC_Vec3<float>& point) { tr.br = br.tr = point; }
    void SetEdgePointB(const ZC_Vec3<float>& point) { bl.br = br.bl = point; }
};

struct Square
{
    ZC_Vec3<float> edgeCenterL,
        edgeCenterT,
        edgeCenterR,
        edgeCenterB;
    bool isEdgePointL = false,  //  is left edge point calculated
        isEdgePointT = false,
        isEdgePointR = false,
        isEdgePointB = false;
};

struct VertexData
{
    NewSquare& rNS;
    ZC_Vec3<float>& rNewPoint,
        & rEC1,
        & rEC2,
        & rCorner1,
        & rCorner2;
    void(NewSquare::*pSetEP1)(const ZC_Vec3<float>&);
    void(NewSquare::*pSetEP2)(const ZC_Vec3<float>&);
    bool& rIsEP1,
        rIsEP2;

    VertexData(NewSquare& _rNewSquare,
                ZC_Vec3<float>& _rNewPoint,
                ZC_Vec3<float>& _rEdgeCenter1,
                ZC_Vec3<float>& _rEdgeCenter2,
                ZC_Vec3<float>& _rCorner1,
                ZC_Vec3<float>& _rCorner2,
                void(NewSquare::*_pSetEdgePoint1)(const ZC_Vec3<float>&),
                void(NewSquare::*_pSetEdgePoint2)(const ZC_Vec3<float>&),
                bool& _rIsEdgePoint1,
                bool& _rIsEdgePoint2)
    : rNS(_rNewSquare),
    rNewPoint(_rNewPoint),
    rEC1(_rEdgeCenter1),
    rEC2(_rEdgeCenter2),
    rCorner1(_rCorner1),
    rCorner2(_rCorner2),
    pSetEP1(_pSetEdgePoint1),
    pSetEP2(_pSetEdgePoint2),
    rIsEP1(_rIsEdgePoint1),
    rIsEP2(_rIsEdgePoint2)
    {}
};

struct CC
{
    struct Vertex
    {
        ZC_Vec3<float> vertex;
        std::list<VertexData> vdList;

        bool operator == (const ZC_Vec3<float>& _v) const noexcept
        {
            return vertex == _v;
        }
    };

    ZC_DA<Square> squares;
    NewSquare* newSquares;
    size_t squaresCount;
    std::list<Vertex> vertices;

    CC(ZC_DA<ZC_Quad>& quads)
        : squares(quads.size),
        newSquares(new NewSquare[quads.size])
    {
        for (size_t _squaresIndex = 0; _squaresIndex < quads.size; ++_squaresIndex)
        {
            Square& rSSq = squares[_squaresIndex];
            ZC_Quad* pQSq = quads.pHead;
            rSSq.edgeCenterL = (pQSq->bl + pQSq->tl) / 2.f;
            rSSq.edgeCenterT = (pQSq->tl + pQSq->tr) / 2.f;
            rSSq.edgeCenterR = (pQSq->tr + pQSq->br) / 2.f;
            rSSq.edgeCenterB = (pQSq->br + pQSq->bl) / 2.f;
            NewSquare& rNS = newSquares[_squaresIndex];
            rNS.SetCenter(quads.pHead->Center());
            Add(pQSq->bl, VertexData(rNS, rNS.bl.bl, rSSq.edgeCenterB, rSSq.edgeCenterL, pQSq->br, pQSq->tl,
                &NewSquare::SetEdgePointB, &NewSquare::SetEdgePointL, rSSq.isEdgePointB, rSSq.isEdgePointL));
            Add(pQSq->tl, VertexData(rNS, rNS.tl.tl, rSSq.edgeCenterT, rSSq.edgeCenterL, pQSq->tr, pQSq->bl,
                &NewSquare::SetEdgePointT, &NewSquare::SetEdgePointL, rSSq.isEdgePointT, rSSq.isEdgePointL));
            Add(pQSq->tr, VertexData(rNS, rNS.tr.tr, rSSq.edgeCenterT, rSSq.edgeCenterR, pQSq->tl, pQSq->br,
                &NewSquare::SetEdgePointT, &NewSquare::SetEdgePointR, rSSq.isEdgePointT, rSSq.isEdgePointR));
            Add(pQSq->br, VertexData(rNS, rNS.br.br, rSSq.edgeCenterB, rSSq.edgeCenterR, pQSq->bl, pQSq->tr,
                &NewSquare::SetEdgePointB, &NewSquare::SetEdgePointR, rSSq.isEdgePointB, rSSq.isEdgePointR));
        }
    }
    
    void Add(const ZC_Vec3<float>& vertex, const VertexData& vData)
    {
        auto pVertex = ZC_Find(vertices, vertex);
        if (pVertex) pVertex->vdList.emplace_back(vData);
        else vertices.emplace_back(Vertex{vertex, {vData}});
    }

    //  Catmull Clark https://en.wikipedia.org/wiki/Catmull%E2%80%93Clark_subdivision_surface
    ZC_DA<ZC_Quad> Calculate()
    {
        for (auto& vert : vertices)
        {
            size_t faces = vert.vdList.size();
            auto vdI = vert.vdList.begin();
            if (faces == 1)   //  point alone
            {
                //  set edges points as edges center
                (vdI->rNS.*vdI->pSetEP1)(vdI->rEC1);
                (vdI->rNS.*vdI->pSetEP2)(vdI->rEC2);
                //  move the center in the direction of the corner + edge centers, by an average length to the center of each edge
                //  (If the figure is a square, the result will be movement by the radius of the inscribed circle)
                vdI->rNewPoint = ZC_Vec::MoveByLength(vdI->rNS.GetCenter(), (vert.vertex + vdI->rEC1 + vdI->rEC2) / 3.f - vdI->rNS.GetCenter(),
                    (ZC_Vec::Length(vdI->rNS.GetCenter() - vdI->rEC1) + ZC_Vec::Length(vdI->rNS.GetCenter() - vdI->rEC2)) / 2.f);
                continue;
            }
    
            ZC_Vec3<float> centersSum,
                commonEdgesCentersSum,
                singleEgdesCentersSum;
            unsigned short commonEdges = 0;
            unsigned short singleEdges = 0;
            //  find edges points
            for (; vdI != vert.vdList.end(); ++vdI)
            {
                centersSum += vdI->rNS.GetCenter();
                for (auto vdI2 = vdI; vdI2 != vert.vdList.end() && (!vdI->rIsEP1 || !vdI->rIsEP2); ++vdI2)
                {
                    if (!vdI->rIsEP1 && !vdI2->rIsEP1 && vdI->rEC1 == vdI2->rEC1)   //  common edge, calculate edge point
                        CommonEdgeCenter(commonEdgesCentersSum, vdI->rEC1, (vert.vertex + vdI->rCorner1 + vdI->rNS.GetCenter() + vdI2->rNS.GetCenter()) / 4.f,
                            vdI->rIsEP1, vdI2->rIsEP1, vdI->rNS, vdI2->rNS, vdI->pSetEP1, vdI2->pSetEP1, commonEdges);
                    if (!vdI->rIsEP1 && !vdI2->rIsEP2 && vdI->rEC1 == vdI2->rEC2)
                        CommonEdgeCenter(commonEdgesCentersSum, vdI->rEC1, (vert.vertex + vdI->rCorner1 + vdI->rNS.GetCenter() + vdI2->rNS.GetCenter()) / 4.f,
                            vdI->rIsEP1, vdI2->rIsEP2, vdI->rNS, vdI2->rNS, vdI->pSetEP1, vdI2->pSetEP2, commonEdges);
                    if (!vdI->rIsEP2 && !vdI2->rIsEP1 && vdI->rEC2 == vdI2->rEC1)
                        CommonEdgeCenter(commonEdgesCentersSum, vdI->rEC2, (vert.vertex + vdI->rCorner2 + vdI->rNS.GetCenter() + vdI2->rNS.GetCenter()) / 4.f,
                            vdI->rIsEP2, vdI2->rIsEP1, vdI->rNS, vdI2->rNS, vdI->pSetEP2, vdI2->pSetEP1, commonEdges);
                    if (!vdI->rIsEP2 && !vdI2->rIsEP2 && vdI->rEC2 == vdI2->rEC2)
                        CommonEdgeCenter(commonEdgesCentersSum, vdI->rEC2, (vert.vertex + vdI->rCorner2 + vdI->rNS.GetCenter() + vdI2->rNS.GetCenter()) / 4.f,
                            vdI->rIsEP2, vdI2->rIsEP2, vdI->rNS, vdI2->rNS, vdI->pSetEP2, vdI2->pSetEP2, commonEdges);
                }  //  no common edges, set edge point as edge center
                if (!vdI->rIsEP1) SingleEdgeCenter(vdI->rEC1, vdI->rIsEP1, vdI->rNS, vdI->pSetEP1, singleEgdesCentersSum, singleEdges);
                if (!vdI->rIsEP2) SingleEdgeCenter(vdI->rEC2, vdI->rIsEP2, vdI->rNS, vdI->pSetEP2, singleEgdesCentersSum, singleEdges);
            }

            ZC_Vec3<float> newPoint;
            if (singleEdges == 0) newPoint = ((centersSum / faces) + (commonEdgesCentersSum / commonEdges) * 2.f + vert.vertex * (commonEdges - 3.f)) / commonEdges;  //  barycenter
            else if (singleEdges == 2) newPoint = (singleEgdesCentersSum + vert.vertex * 2.f) / 4.f;
            else newPoint = vert.vertex;

            for (auto& rvd : vert.vdList) rvd.rNewPoint = newPoint;
        }

        ZC_DA<ZC_Quad> result(reinterpret_cast<ZC_Quad*>(newSquares), squares.size * 4);
        squares.pHead = nullptr;
        return result;
    }

    void SingleEdgeCenter(const ZC_Vec3<float>& point, bool& rIsEP, NewSquare& rNS, void(NewSquare::*pSet)(const ZC_Vec3<float>&),
        ZC_Vec3<float>& rSECS, unsigned short& rSingleEdges)
    {
        rIsEP = true;
        (rNS.*pSet)(point);
        rSECS += point;
        ++rSingleEdges;
    }

    void CommonEdgeCenter(ZC_Vec3<float>& rCECSum, ZC_Vec3<float>& rCEC, const ZC_Vec3<float>& point, bool& rIsEP1, bool& rIsEP2, NewSquare& rNS1, NewSquare& rNS2,
        void(NewSquare::*pSet1)(const ZC_Vec3<float>&), void(NewSquare::*pSet2)(const ZC_Vec3<float>&), unsigned short& rCommonEdges)
    {
        rCECSum += rCEC;
        rIsEP1 = true;
        (rNS1.*pSet1)(point);
        rIsEP1 = true;
        (rNS2.*pSet2)(point);
        ++rCommonEdges;
    }
};
//  ZCR_Camera scroll and IGWindow for axis
#include <ZC/Tools/Math/ZC_Vec2.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#include <ZC/Video/OpenGL/ZC_OpenGL.h>

#include <ZC/File/ZC_File.h>
#include <ZC/Tools/Math/ZC_Math.h>

enum ZC_TextAlignment
{
    ZC_TA_Left,
    ZC_TA_Right,
    ZC_TA_Center,
};

enum ZC_FontName
{
    Arial,
};

struct ZC_Fonts
{
    struct NameHeight
    {
        ZC_FontName name;
        size_t height;

        bool operator < (const NameHeight& fs) const noexcept
        {
            return name < fs.name || height < fs.height;    //  && || ???
        }
    };

    struct Character
    {
        float advX; 	// advance.x
        float advY;	    // advance.y

        float bitmapW;	    // bitmap.width;
        float bitmapH;  	// bitmap.height;

        float bitmapLeft;	// bitmap_left;
        float bitmapTop;	// bitmap_top;

        float texX_left;	    // x offset of glyph in texture coordinates
        float texY_bottom;	    // y offset of glyph in texture coordinates
        float texX_right;	    // x offset of glyph in texture coordinates
        float texY_top;	        // y offset of glyph in texture coordinates
    };

    struct Font
    {
        struct Point
        {
            float x,
                y;
            ushort s,  // texCoords X
                t;  // texCoords Y
        };

        ZC_Texture texture;
        std::map<char, Character> characters;

        struct LineData
        {
            size_t coordsStartIndex,
                coordsCount;
            float mustBeUp_Y,
                width;
        };

        enum Origin
        {
            center,
            bottomLeft,
            bottomCenter
        };

        //  makeOriginCenter - make origin center of text (by default origin bottom left for indent translates in ZC_TextWindow)
        std::vector<Point> FillCoords(const std::string& text, Origin origin, ZC_TextAlignment alignment, float& rTextWidth, float& rTextHeight)
        {
            auto newLineCount = std::count(text.begin(), text.end(), '\n');
            std::vector<LineData> linesData;
            linesData.reserve(newLineCount + 1);   //  + 1 -> minimum 1 line

            std::vector<Point> coords;
            coords.reserve((text.length() - std::count(text.begin(), text.end(), ' ') - newLineCount) * 4); //  4 -> vertices count, except ' ' and '\n'

            FillCoordsAndLines(text, coords, linesData, rTextWidth, rTextHeight);

            switch (origin)
            {
            case Origin::center: CalcutateXYOriginCenter(linesData, coords, alignment, rTextWidth, rTextHeight); break;
            case Origin::bottomLeft: CalcutateXYOriginBottomLeft(linesData, coords, alignment, rTextWidth); break;
            case Origin::bottomCenter: CalcutateXYOriginBottomCenter(linesData, coords, alignment, rTextWidth); break;
            }

            return coords;
        }

        void FillCoordsAndLines(const std::string& text, std::vector<Point>& rCoords, std::vector<LineData>& rLinesData, float& rTotalWidth, float& rTotalHeight)
        {
            float startX = 0,
                startY = 0,
                lineHeightWithoughtBottomTail = 0,
                lineHeightBottomTail = 0;
            size_t coordsStartIndex = 0,
                coordsCount = 0;
            for (auto c : text)
            {
                if (c == '\n')
                {
                    AddLine(lineHeightBottomTail, lineHeightWithoughtBottomTail, rLinesData, coordsStartIndex, coordsCount, startX, rTotalWidth, rTotalHeight);
                    startX = 0;
                    startY = 0;
                    lineHeightWithoughtBottomTail = 0;
                    lineHeightBottomTail = 0;
                    coordsCount = 0; 
                    continue;
                }

                const Character& ch = characters[c];
                float x = startX + ch.bitmapLeft,
                    y = startY + (ch.bitmapH - (ch.bitmapH - ch.bitmapTop)),
                    w = ch.bitmapW,
                    h = ch.bitmapH;

                startX += ch.advX;
                // startY += ch.advY;

                float mustBeUp = h - y;
                if (mustBeUp > lineHeightBottomTail) lineHeightBottomTail = mustBeUp;

                float symbolsHeightWithoughtDownTail = h - mustBeUp;
                if (lineHeightWithoughtBottomTail < symbolsHeightWithoughtDownTail) lineHeightWithoughtBottomTail = symbolsHeightWithoughtDownTail;

                if (!w || !h) continue;   //  whitespace

                ushort texCoord_l = ZC_PackTexCoordFloatToUShort(ch.texX_left);
                ushort texCoord_r = ZC_PackTexCoordFloatToUShort(ch.texX_right);
                ushort texCoord_t = ZC_PackTexCoordFloatToUShort(ch.texY_top);
                ushort texCoord_b = ZC_PackTexCoordFloatToUShort(ch.texY_bottom);

                rCoords.emplace_back(Point{ x, y, texCoord_l, texCoord_b });            //  bl  full texture of symbols is Y inversed!
                rCoords.emplace_back(Point{ x + w, y - h, texCoord_r, texCoord_t });    //  tr  full texture of symbols is Y inversed!
                rCoords.emplace_back(Point{ x, y - h, texCoord_l, texCoord_t });        //  tl  full texture of symbols is Y inversed!
                rCoords.emplace_back(Point{ x + w, y, texCoord_r, texCoord_b });        //  br  full texture of symbols is Y inversed!

                coordsCount += 4;
            }

            AddLine(lineHeightBottomTail, lineHeightWithoughtBottomTail, rLinesData, coordsStartIndex, coordsCount, startX, rTotalWidth, rTotalHeight);
        }

        void CalcutateXYOriginBottomLeft(std::vector<LineData>& rLinesData, std::vector<Point>& rCoords, ZC_TextAlignment alignment, float textWidth)
        {
            UpY(rLinesData, rCoords);

            if (alignment == ZC_TA_Left) return;    //  Alignment_Left is default

            for (auto& lineData : rLinesData)    //  X alignment
            {
                if (lineData.width == textWidth) continue;
                float offsetX = alignment == ZC_TA_Right ? textWidth - lineData.width : (textWidth - lineData.width) / 2.f;  //  otherwise Alignment_Center
                for (size_t i = lineData.coordsStartIndex; i < lineData.coordsStartIndex + lineData.coordsCount; ++i)
                    rCoords[i].x += offsetX;                
            }
        }

        void CalcutateXYOriginCenter(std::vector<LineData>& rLinesData, std::vector<Point>& rCoords,
            ZC_TextAlignment alignment, float textWidth, float textHeight)
        {
            float halfTextHeight = textHeight / 2.f;
            for (auto& lineData : rLinesData)    // Y alignment (down tails and next lines height)
            {
                for (size_t i = lineData.coordsStartIndex; i < lineData.coordsStartIndex + lineData.coordsCount; ++i)
                    rCoords[i].y += lineData.mustBeUp_Y - halfTextHeight;    //  - halfTextHeight (make texts Y origin center)
            }
            
            MakeCenterX(rLinesData, rCoords, alignment, textWidth);
        }

        void CalcutateXYOriginBottomCenter(std::vector<LineData>& rLinesData, std::vector<Point>& rCoords, ZC_TextAlignment alignment, float textWidth)
        {
            UpY(rLinesData, rCoords);
            MakeCenterX(rLinesData, rCoords, alignment, textWidth);
        }

        void UpY(std::vector<LineData>& rLinesData, std::vector<Point>& rCoords)
        {
            for (auto& lineData : rLinesData)    // Y alignment (down tails and next lines height)
            {
                if (lineData.mustBeUp_Y == 0) continue;
                for (size_t i = lineData.coordsStartIndex; i < lineData.coordsStartIndex + lineData.coordsCount; ++i)
                    rCoords[i].y += lineData.mustBeUp_Y;
            }
        }

        void MakeCenterX(std::vector<LineData>& rLinesData, std::vector<Point>& rCoords, ZC_TextAlignment alignment, float textWidth)
        {
            float halfTextWidth = textWidth / 2.f;
            for (auto& lineData : rLinesData)    //  X alignment
            {
                float offsetX = -halfTextWidth;     //  make texts X origin center
                if (alignment != ZC_TA_Left && lineData.width != textWidth)
                    offsetX += alignment == ZC_TA_Right ? textWidth - lineData.width : (textWidth - lineData.width) / 2.f;  //  otherwise Alignment_Center
                for (size_t i = lineData.coordsStartIndex; i < lineData.coordsStartIndex + lineData.coordsCount; ++i)
                    rCoords[i].x += offsetX;                
            }
        }

        void AddLine(float lineHeightDownTail, float lineHeightWithoughtButtomTail, std::vector<LineData>& rLinesData, size_t& rCoordsStartIndex,
            size_t coordsCount, float startX, float& rTotalWidth, float& rTotalHeight)
        {
            float previousLinesMustBeUp_Y = lineHeightDownTail + lineHeightWithoughtButtomTail;
            for (auto& lineData : rLinesData) lineData.mustBeUp_Y += previousLinesMustBeUp_Y;
            
            rLinesData.emplace_back(LineData{ rCoordsStartIndex, coordsCount, lineHeightDownTail, startX });    //  last startX same as line width
            if (startX > rTotalWidth) rTotalWidth = startX;
            rCoordsStartIndex += coordsCount;

            rTotalHeight += previousLinesMustBeUp_Y;
        }
    };

    static inline std::map<NameHeight, Font> fonts;
    static const uint texMaxW = 1024,
        borderPixel = 1;
    static const ulong firstASCII = 32,
        lastASCII = 127;

    static void Load(NameHeight* names, size_t namesCount)
    {
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) ZC_ErrorLogger::Err("Fail FT_Init_FreeType()!", __FILE__, __LINE__);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (size_t namesIndex = 0; namesIndex < namesCount; ++namesIndex)
        {
            auto fontsIter = fonts.find(names[namesIndex]);
            if (fontsIter != fonts.end()) continue;

            FT_Face face;
            if (FT_New_Face(ft, GetPath(names[namesIndex].name).c_str(), 0, &face))
            {
                ZC_ErrorLogger::Err("Fail FT_New_Face()!", __FILE__, __LINE__);
                continue;
            }

            if (FT_Set_Pixel_Sizes(face, 0, names[namesIndex].height))
            {
                ZC_ErrorLogger::Err("Fail FT_Set_Pixel_Sizes()!", __FILE__, __LINE__);
                continue;
            }

            fonts.emplace(names[namesIndex], MakeFont(face));
        }

        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }

    static Font* GetFont(NameHeight name)
    {
        auto fontsIter = fonts.find(name);
        if (fontsIter == fonts.end()) ZC_ErrorLogger::Err("Error in GetFont() Font wasn't loaded!", __FILE__,__LINE__);
        return &(fontsIter->second);
    }

    static std::string GetPath(ZC_FontName name)
    {
        static const ZC_FSPath ZC_fontsPath = ZC_FSPath(ZC_ZCDirPath).append("fonts");
        switch (name)
        {
        case Arial: return ZC_FSPath(ZC_fontsPath).append("arial.ttf").string();
        default: return "";
        }
    }

    static Font MakeFont(const FT_Face& face)
    {
        uint texW = 0,
            texH = 0;
        CalculateTextureSize(texW, texH, face);

        ZC_Texture texture(GL_TEXTURE_2D, GL_RED, texW, texH, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
        texture.Bind();

        float texX = 0,
            texY = 0;
        unsigned rowH = 0;
        FT_GlyphSlot glyph = face->glyph;

        std::map<char, Character> characters;
    
        for (auto symbol = firstASCII; symbol < lastASCII; ++symbol)
        {
            if (FT_Load_Char(face, symbol, FT_LOAD_RENDER)) continue;

            if (texX + glyph->bitmap.width + borderPixel > texMaxW)
            {
                texY += rowH + borderPixel;
                rowH = 0;
                texX = 0;
            }
            
            if (glyph->bitmap.width != 0 && glyph->bitmap.rows != 0)
                glTexSubImage2D(GL_TEXTURE_2D, 0, texX, texY, glyph->bitmap.width, glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);
                
            float texX_left = texX / static_cast<float>(texW),
                texY_bottom = texY / static_cast<float>(texH);
            characters.emplace(symbol, Character
                {
                    static_cast<float>(glyph->advance.x >> 6),
                    static_cast<float>(glyph->advance.y >> 6),
                    static_cast<float>(glyph->bitmap.width),
                    static_cast<float>(glyph->bitmap.rows),
                    static_cast<float>(glyph->bitmap_left),
                    static_cast<float>(glyph->bitmap_top),
                    texX_left,
                    texY_bottom,
                    texX_left + (static_cast<float>(glyph->bitmap.width) / texW),
                    texY_bottom + (static_cast<float>(glyph->bitmap.rows) / texH)
                });

            texX += glyph->bitmap.width + borderPixel;
            rowH = std::max(glyph->bitmap.rows, rowH);
        }
        texture.Unbind();

        return { std::move(texture), std::move(characters) };
    }

    static void CalculateTextureSize(uint& texW, uint& texH, const FT_Face& face)
    {
        unsigned rowW = 0,
            rowH = 0;
        FT_GlyphSlot glyph = face->glyph;

        for (auto symbol = firstASCII; symbol < lastASCII; ++symbol)
        {
            if (FT_Load_Char(face, symbol, FT_LOAD_RENDER))
            {
                ZC_ErrorLogger::Err("Fail FT_Load_Char()!", __FILE__, __LINE__);
                continue;
            }

            if (rowW + glyph->bitmap.width + borderPixel > texMaxW)
            {
                texW = std::max(rowW, texW);
                texH += rowH + borderPixel;
                rowW = 0;
                rowH = 0;
            }

            rowW += glyph->bitmap.width + borderPixel;
            rowH = std::max(rowH, glyph->bitmap.rows);
        }

        texW = std::max(texW, rowW);
        texH += rowH + borderPixel;
    }
};



template<typename TpHead>
void FillTriangleElements(TpHead pElementsHead, size_t elementsSize, size_t quadsElementsCount)
{
    size_t elementsI = 0,
        index = 0;
    for ( ; elementsI < quadsElementsCount; elementsI += 6)
    {
        pElementsHead[elementsI] = pElementsHead[elementsI + 3] = index++;
        pElementsHead[elementsI + 1] = pElementsHead[elementsI + 5] = index++;
        pElementsHead[elementsI + 2] = index++;
        pElementsHead[elementsI + 4] = index++;
    }
    while (elementsI < elementsSize)
    {
        pElementsHead[elementsI++] = index++;
        pElementsHead[elementsI++] = index++;
        pElementsHead[elementsI++] = index++;
    }
}

/*
Create dynamic array of elements for drawing quads and(or) triangles. Im vbo vertices(corners) of quad must be in order [bl -> tr -> tl -> br].

Params:
rElementsCount - set count of elements in ebo (ZC_DrawElements>(..., elementsCount, ..., ...);)
rElementsType - set elements type in ebo (ZC_DrawElements>(..., ..., elementsType, ...);)
quadsCount - count of quads in vbo.
trianglesCount - count of triangles in vbo.

Return:
Array of element in uchar format. Using:
    ZC_Buffer ebo(GL_ELEMENT_ARRAY_BUFFER);
    ebo.BufferData(elements.size, elements.Begin(), ...);
*/
ZC_DA<uchar> GetTriangleElements(size_t& rElementsCount, GLenum& rElementsType, size_t quadsCount, size_t trianglesCount)
{
    size_t quadsElementsCount = quadsCount * 6,     //  6 elements in ebo on one quad
        trianglesElementsCount = trianglesCount * 3;     //  3 elements in ebo on one triangle
    rElementsCount = quadsElementsCount + trianglesElementsCount;  
    size_t verticesInVBO = (quadsCount * 4) + trianglesElementsCount,     //  4 vertices in vbo on one quad
        storingTypeSize = 0;
    ZC_Buffer::GetElementsData(verticesInVBO - 1, storingTypeSize, rElementsType);
    ZC_DA<uchar> elements(storingTypeSize * rElementsCount);
    switch (storingTypeSize)
    {
    case 1: FillTriangleElements(elements.pHead, elements.size, quadsElementsCount); break;
    case 2: FillTriangleElements(reinterpret_cast<ushort*>(elements.pHead), elements.size / 2, quadsElementsCount); break;
    case 4: FillTriangleElements(reinterpret_cast<uint*>(elements.pHead), elements.size / 4, quadsElementsCount); break;
    }
    return elements;
}

struct ZC_TextData
{
public:
    virtual ~ZC_TextData() = default;

    void NeedDraw(bool needDraw)
    {
        needDraw ? rsController.SwitchToLvl(rendererLevel)
            : rsController.SwitchToLvl(ZC_RL_None);
    }

    void SetColor(const ZC_Vec3<float>& color)
    {
        uint colorUint = ZC_PackColorFloatToUInt(color[0], color[1], color[2]);
        ZC_RSPDUniformData unColor(ZC_UN_unColor, &colorUint);
        rsController.SetData(ZC_RSPDC_uniforms, &unColor);
    }

    void SetText(const std::string& _text)
    {
        if (spTextSharedData->text == _text) return;
        spTextSharedData->text = _text;

        //  change data in ZC_DrawElements don't changing pointer in spTextSharedData->rendererSet.upGLDraw
        dynamic_cast<ZC_DrawElements&>(*(spTextSharedData->rendererSet.upGLDraw)) =     //  vbo - second in buffers (firsts added); ebo - first in buffers (seconds added)
            CalculateAndSetTextData(*(++(spTextSharedData->rendererSet.buffers.begin())), *(spTextSharedData->rendererSet.buffers.begin()),
            spTextSharedData->text, spTextSharedData->alignment);
        SetNewTextSize();
    }

    void SetAlignment(ZC_TextAlignment _alignment)
    {
        if (spTextSharedData->alignment == _alignment) return;
        spTextSharedData->alignment = _alignment;

        //  change data in ZC_DrawElements don't changing pointer in spTextSharedData->rendererSet.upGLDraw
        dynamic_cast<ZC_DrawElements&>(*(spTextSharedData->rendererSet.upGLDraw)) =     //  vbo - second in buffers (firsts added); ebo - first in buffers (seconds added)
            CalculateAndSetTextData(*(++(spTextSharedData->rendererSet.buffers.begin())), *(spTextSharedData->rendererSet.buffers.begin()),
            spTextSharedData->text, spTextSharedData->alignment);
    }

    //  If need to change text and alignment at the same time, use this function. Calling SetText() and SetAlignment() separately is less effective.
    void SetTextAndAlignment(const std::string& _text, ZC_TextAlignment _alignment)
    {
        bool recalculateText = false;
        if (spTextSharedData->text != _text)
        {
            spTextSharedData->text = _text;
            recalculateText = true;
        }
        bool recalculateAlignment = false;
        if (spTextSharedData->alignment != _alignment)
        {
            spTextSharedData->alignment = _alignment;
            recalculateAlignment = true;
        }
        if (recalculateText || recalculateAlignment)
            dynamic_cast<ZC_DrawElements&>(*(spTextSharedData->rendererSet.upGLDraw)) =     //  vbo - second in buffers (firsts added); ebo - first in buffers (seconds added)
                CalculateAndSetTextData(*(++(spTextSharedData->rendererSet.buffers.begin())), *(spTextSharedData->rendererSet.buffers.begin()),
                spTextSharedData->text, spTextSharedData->alignment);

        if (recalculateText) SetNewTextSize();
    }

protected:
    struct SharedData
    {
        std::string text;
        ZC_TextAlignment alignment;
        ZC_RendererSet rendererSet;
    };

    typedef typename ZC_Fonts::Font Font;
    typedef typename Font::Origin FontOrigin;
    Font* pFont;
    FontOrigin fontOrigin;
    ZC_sptr<SharedData> spTextSharedData;
    ZC_RSController rsController;
    float textWidth,
        textHeight;
    ZC_RendererLevel rendererLevel;

    ZC_TextData(typename ZC_ShProgs::ShPInitSet* pShPIS, FontOrigin _fontOrigin, typename ZC_Fonts::NameHeight fontData, const std::string& _text,
            ZC_TextAlignment _alignment, ZC_RendererLevel _rendererLevel)
        : pFont(ZC_Fonts::GetFont(fontData)),
        fontOrigin(_fontOrigin),
        spTextSharedData(ZC_sptrMake<SharedData>(_text, _alignment, MakeRendererSet(pShPIS, _text, _alignment))),
        rsController(spTextSharedData->rendererSet.MakeZC_RSController()),
        rendererLevel(_rendererLevel)
    {
        rsController.pTexture = &(pFont->texture);  //  the texture pointer is manually set because in the case of Text one texture can be used in multiple objects and is stored in Font::Font and not in ZC_RendererSet::texSets
        rsController.texturesCount = 1;

        NeedDraw(true);
    }

    ZC_TextData(const ZC_TextData& td)
        : pFont(td.pFont),
        fontOrigin(td.fontOrigin),
        spTextSharedData(td.spTextSharedData),
        rsController(td.rsController.MakeCopy()),
        textWidth(td.textWidth),
        textHeight(td.textHeight),
        rendererLevel(td.rendererLevel)
    {
        NeedDraw(true);
    }

private:
    virtual void SetNewTextSize() {};
    
    ZC_RendererSet MakeRendererSet(typename ZC_ShProgs::ShPInitSet* pShPIS, const std::string& _text, ZC_TextAlignment _alignment)
    {
        ZC_Buffer vbo(GL_ARRAY_BUFFER);
        ZC_Buffer ebo(GL_ELEMENT_ARRAY_BUFFER);
        ZC_VAO vao;
        vao.Config(pShPIS->vaoConfigData, vbo, &ebo, 0, 0);

        auto upGLDraw = ZC_uptrMakeFromChild<ZC_GLDraw, ZC_DrawElements>(CalculateAndSetTextData(vbo, ebo, _text, _alignment));

        std::forward_list<ZC_Buffer> buffers;
        buffers.emplace_front(std::move(vbo));
        buffers.emplace_front(std::move(ebo));

        return ZC_RendererSet(pShPIS, std::move(vao), std::move(upGLDraw), std::move(buffers));
    }

    ZC_DrawElements CalculateAndSetTextData(ZC_Buffer& rVBO, ZC_Buffer& rEBO, const std::string& text, ZC_TextAlignment alignment)
    {
        textWidth = 0;
        textHeight = 0;
        auto pointsCoords = pFont->FillCoords(text, fontOrigin, alignment, textWidth, textHeight);

        rVBO.BufferData(pointsCoords.size() * sizeof(typename Font::Point), &(pointsCoords[0]), GL_STATIC_DRAW);
  
        size_t elementsCount = 0;
        GLenum elementsType = 0;
        ZC_DA<uchar> elements = GetTriangleElements(elementsCount, elementsType, pointsCoords.size() / 4, 0);

        rEBO.BufferData(elements.size, elements.Begin(), GL_STATIC_DRAW);
        //  creates on stack ZC_DrawElements cause in MakeRendererSet() will created in heap ZC_uptr<ZC GLDraw>, in other functions updates data created in MakeRendererSet()
        return { GL_TRIANGLES, static_cast<int>(elementsCount), elementsType, 0 };
    }
};

//  Class for rendering text into the window.
class ZC_TextWindow : public ZC_WindowOrthoIndent, public ZC_TextData
{
public:
    /*
    Params:
    fontData - fonts name and size.
    _text - text for rendering (supports'\n' new line symbol).
    _alignment - alignment across text consisting of several lines of different lengths.
    color - texts color.
    windowIndentX - value of horizontal indent from border of global window. If used IndentFlag:
        ZC_WOIF__X_Left_Pixel, ZC_WOIF__X_Right_Pixel -> value must be not negative, otherwise sets 0.f;
        ZC_WOIF__X_Left_Percent, ZC_WOIF__X_Right_Percent -> value must be 0.0f - 1.f (where 1.f is 100%);
        ZC_WOIF__X_Center -> value no metter.
    windowIndentY - value of vertival indent from border of global window. If used IndentFlag: 
        ZC_WOIF__Y_Top_Pixel, ZC_WOIF__Y_Bottom_Pixel -> value must be not negative, otherwise sets 0.f
        ZC_WOIF__Y_Top_Percent, ZC_WOIF__Y_Bottom_Percent -> value must be 0.0f - 1.f (where 1.f is 100%);
        ZC_WOIF__Y_Center -> value no metter.
    indentFlags - flags of indent horizontal(X) and vertical(Y) from border of global window to IGWindow. Must be set one flag for X and one flag for Y. Example: X_Left_Pixel | Y_Top_Pixel.
    */
    ZC_TextWindow(typename ZC_Fonts::NameHeight fontData, const std::string& _text, ZC_TextAlignment _alignment,
            float windowIndentX, float windowIndentY, ZC_WindowOrthoIndentFlags indentFlags)
        : ZC_WindowOrthoIndent(false, 0, 0, windowIndentX, windowIndentY, indentFlags),
        ZC_TextData(ZC_ShProgs::Get(ZC_ShProgs::Name::ZC_TextWindow), FontOrigin::bottomLeft, fontData, _text, _alignment, ZC_RendererLevels::TextWindow)
    {
        SetNewTextSize();

        ZC_RSPDUniformData unPosition(ZC_UN_unPositionWindow, this->currentIndents);
        rsController.SetData(ZC_RSPDC_uniforms, &unPosition);
    }

    /*
    Params:
    windowIndentX - value of horizontal indent from border of global window. If used IndentFlag:
        ZC_WOIF__X_Left_Pixel, ZC_WOIF__X_Right_Pixel -> value must be not negative, otherwise sets 0.f;
        ZC_WOIF__X_Left_Percent, ZC_WOIF__X_Right_Percent -> value must be 0.0f - 1.f (where 1.f is 100%);
        ZC_WOIF__X_Center -> value no metter.
    windowIndentY - value of vertival indent from border of global window. If used IndentFlag: 
        ZC_WOIF__Y_Top_Pixel, ZC_WOIF__Y_Bottom_Pixel -> value must be not negative, otherwise sets 0.f
        ZC_WOIF__Y_Top_Percent, ZC_WOIF__Y_Bottom_Percent -> value must be 0.0f - 1.f (where 1.f is 100%);
        ZC_WOIF__Y_Center -> value no metter.
    indentFlags - flags of indent horizontal(X) and vertical(Y) from border of global window to IGWindow. Must be set one flag for X and one flag for Y. Example: X_Left_Pixel | Y_Top_Pixel.
    */
    void SetIndentData(float windowIndentX, float windowIndentY, ZC_WindowOrthoIndentFlags indentFlags)
    {
        this->SetNewIndentParams(windowIndentX, windowIndentY, indentFlags);
    }

    /*
    Makes a copy of the text.
    The methods affect only the current copy: NeedDraw(), SetColor(), SetIndentData().
    Methods that include effects on all copies: SetText() SetAlignment() SetTextAndAlignment().
    */
    ZC_TextWindow MakeCopy(float windowIndentX, float windowIndentY, ZC_WindowOrthoIndentFlags indentFlags)
    {
        ZC_TextWindow copy = { *this };
        copy.SetIndentData(windowIndentX, windowIndentY, indentFlags);
        return copy;
    } 

private:
    ZC_TextWindow(const ZC_TextWindow& tw)
        : ZC_WindowOrthoIndent(dynamic_cast<const ZC_WindowOrthoIndent&>(tw)),
        ZC_TextData(dynamic_cast<const ZC_TextData&>(tw))
    {
        rsController.SetUniformsData(ZC_UN_unPositionWindow, this->currentIndents);
    }

    void SetNewTextSize() override
    {
        this->SetNewSize(textWidth, textHeight);    //  calculates text indents into the window and store text size in ZC_WindowOrthoIndent
    }
};

//  Class for rendering text into the scene.
class ZC_TextScene : public ZC_TextData
{
public:
    /*
    Params:
    fontData - fonts name and size.
    _text - text for rendering (supports'\n' new line symbol).
    _alignment - alignment across text consisting of several lines of different lengths.
    color - texts color.
    */
    ZC_TextScene(typename ZC_Fonts::NameHeight fontData, const std::string& _text, ZC_TextAlignment _alignment)
        : ZC_TextData(ZC_ShProgs::Get(ZC_ShProgs::Name::ZC_TextScene), FontOrigin::center, fontData, _text, _alignment, ZC_RendererLevels::TextScene)
    {
        rsController.SetUniformsData(ZC_UN_unModel, &(model.Scale(scale, scale, scale)));
    }

    void SetPosition(const ZC_Vec3<float>& pos) noexcept
    {
        if (position == pos) return;
        position = pos;
        RecalculateModelMatrix();
    }

    void SetRotation(float angle, const ZC_Vec3<float>& axises) noexcept
    {
        if (rotationAngle == angle && rotationAxises == axises) return;
        rotationAngle = angle;
        rotationAxises = axises;
        RecalculateModelMatrix();
    }

    void SetScale(float _scale)
    {
        if (scale == _scale) return;
        scale = _scale;
        RecalculateModelMatrix();
    }

    //  This function must be used if more than one parameter of the model matrix(position, rotaion, scale) needs to be changed. 
    void SetModelMatrixData(ZC_Vec3<float>* pPosition, float* pRotationAngle, const ZC_Vec3<float>* pRotationAxises, float* pScale)
    {
        if (pPosition) position = *pPosition;
        if (pRotationAngle && pRotationAxises)
        {
            rotationAngle = *pRotationAngle;
            rotationAxises = *pRotationAxises;
        }
        if (pScale) scale = *pScale;
        RecalculateModelMatrix();
    }

    /*
    Makes a copy of the text.
    The methods affect only the current copy: NeedDraw(), SetColor(), SetPosition(), SetRotation(), SetScale(), SetModelMatrixData().
    Methods that include effects on all copies: SetText() SetAlignment() SetTextAndAlignment().
    */
    ZC_TextScene MakeCopy(ZC_Vec3<float>* pPosition, float* pRotationAngle, const ZC_Vec3<float>* pRotationAxises, float* pScale)
    {
        ZC_TextScene copy = { *this };
        copy.SetModelMatrixData(pPosition, pRotationAngle, pRotationAxises, pScale);
        return copy;
    } 

private:
    ZC_Mat4<float> model { 1.f };
    float scale = 0.01f;
    ZC_Vec3<float> position{ 0.f, 0.f, 0.f };
    float rotationAngle = 0.f;
    ZC_Vec3<float> rotationAxises{ 0.f, 0.f, 0.f };

    ZC_TextScene(const ZC_TextScene& ts)
        : ZC_TextData(dynamic_cast<const ZC_TextData&>(ts)),
        model(ts.model),
        scale(ts.scale),
        position(ts.position),
        rotationAngle(ts.rotationAngle),
        rotationAxises(ts.rotationAxises)
    {
        rsController.SetUniformsData(ZC_UN_unModel, &model);
    }

    void RecalculateModelMatrix()
    {
        model = ZC_Mat4<float>(1.f);
        model.Translate(position);
        if (rotationAngle != 0.f) model.Rotate(rotationAngle, rotationAxises);
        model.Scale(scale, scale, scale);
    }
};
#include <cassert>
struct ZC_TurnToCamera
{
    virtual ~ZC_TurnToCamera()
    {
        bool isErased = ZC_ForwardListErase(allHeirs, this);
        assert(isErased);
        if (isNeedUpdate) ZC_ForwardListErase(needUpdate, this);  
        if (allHeirs.empty()) sconEventsEnd.Disconnect();
    }

    void SetPosition(const ZC_Vec3<float>& pos)
    {
        if (position == pos) return;
        position = pos;
        if (!isNeedUpdate)
        {
            isNeedUpdate = true;
            needUpdate.emplace_front(this);
        }
    }

    void SetScale(float _scale)
    {
        if (scale == _scale) return;
        if (scale != 0.f)   //  need return to 1.0 -> 100%
        {   //  (model[0] / scale) -> return 100% scale, if new scale not null make scale -> (* _scale)
            auto column0 = _scale == 0.f ? model[0] / scale : model[0] / scale * _scale,
                column1 = _scale == 0.f ? model[1] / scale : model[1] / scale * _scale,
                column2 = _scale == 0.f ? model[2] / scale : model[2] / scale * _scale;
            model[0] = column0;
            model[1] = column1;
            model[2] = column2;
        }
        scale = _scale;
    }

protected:
    ZC_Mat4<float> model;
    ZC_Vec3<float> position;
    bool isNeedUpdate = true;
    float scale = 0.f;

    ZC_TurnToCamera(const ZC_Vec3<float>& _position, float _scale)
        : position(_position),
        scale(_scale)
    {
        if (allHeirs.empty()) sconEventsEnd = ZC_Events::ConnectHandleEventsEnd({ &ZC_TurnToCamera::Update });
        allHeirs.emplace_front(this);
        this->CalculateModel(*ZC_Camera::GetCamPos(), (*ZC_Camera::GetUp())[2] == 1.f);
    }

    ZC_TurnToCamera(const ZC_TurnToCamera& ttc)
        : model(ttc.model),
        position(ttc.position),
        isNeedUpdate(ttc.isNeedUpdate),
        scale(ttc.scale)
    {
        allHeirs.emplace_front(this);
        if (isNeedUpdate) needUpdate.emplace_front(this);
    }

private:
    static inline ZC_SConnection sconEventsEnd; //  must be connected to event, which calls after all camera manipulations  !!!!!!!!!!!!
    static inline std::forward_list<ZC_TurnToCamera*> allHeirs;
    static inline std::forward_list<ZC_TurnToCamera*> needUpdate;

    static void Update(float time)
    {
        static ZC_Vec3<float> previousCameraPosition;

        auto currentCamPos = ZC_Camera::GetCamPos();
        bool isNormalVerticalOrientation = (*ZC_Camera::GetUp())[2] == 1.f;
        if (previousCameraPosition == *currentCamPos)
        {
            for (auto pHeir : needUpdate)
            {
                pHeir->CalculateModel(previousCameraPosition, isNormalVerticalOrientation);
            }
        }
        else
        {
            previousCameraPosition = *currentCamPos;
            for (auto pHeir : allHeirs)
            {
                pHeir->CalculateModel(previousCameraPosition, isNormalVerticalOrientation);
            }
        }
        needUpdate.clear();
    }

    void CalculateModel(const ZC_Vec3<float>& camPos, bool isNormalVerticalOrientation)
    {
        auto posToCam = position - camPos;
        ZC_Vec3<float> zeroZ(posToCam[0], posToCam[1], 0);

        auto normPosToCam = ZC_Vec::Normalize(posToCam);
        ZC_Vec3<float> vertivalStartPoint = ZC_Vec::Normalize(ZC_Vec3<float>(posToCam[0], posToCam[1], 0.f));    //  also end point for horizontal rotation
        auto cosVertical = ZC_Vec::Dot(normPosToCam, vertivalStartPoint);
        auto sinVertical = sqrt(1.f - (cosVertical * cosVertical));
        if (posToCam[2] > 0) sinVertical *= -1.f;
        if (!isNormalVerticalOrientation) cosVertical *= -1.f;

        ZC_Vec3<float> horizontalStartPoint(0.f, 1.f, 0.f);
        auto cosHorizontal = ZC_Vec::Dot(horizontalStartPoint, vertivalStartPoint);   //  vertivalStartPoint is normalized end point for horizontal angle
        auto sinHorizontal = sqrt(1.f - (cosHorizontal * cosHorizontal));
        if (posToCam[0] < 0) sinHorizontal *= -1.f;
        if (!isNormalVerticalOrientation)
        {
            cosHorizontal *= -1.f;
            sinHorizontal *= -1.f;
        }

        model = ZC_Mat4<float>(1.f);
        model.Translate(position);
        Rotate(model, cosHorizontal, sinHorizontal, { 0, 0, -1.f });
        Rotate(model, cosVertical, sinVertical, { -1.f, 0, 0 });
        if (scale != 0.f) model.Scale(scale, scale, scale);

        isNeedUpdate = false;
    }

    static void Rotate(ZC_Mat4<float>& model, float cos, float sin, const ZC_Vec3<float>& axise)
    {
        ZC_Vec3<float> temp(axise * (static_cast<float>(1) - cos));

        ZC_Vec3<float> rotateX;
        rotateX[0] = cos + temp[0] * axise[0];
        rotateX[1] = temp[0] * axise[1] + sin * axise[2];
        rotateX[2] = temp[0] * axise[2] - sin * axise[1];
        ZC_Vec4<float> columnX = model[0] * rotateX[0] + model[1] * rotateX[1] + model[2] * rotateX[2];

        ZC_Vec3<float> rotateY;
        rotateY[0] = temp[1] * axise[0] - sin * axise[2];
        rotateY[1] = cos + temp[1] * axise[1];
        rotateY[2] = temp[1] * axise[2] + sin * axise[0];
        ZC_Vec4<float> columnY = model[0] * rotateY[0] + model[1] * rotateY[1] + model[2] * rotateY[2];

        ZC_Vec3<float> rotateZ;
        rotateZ[0] = temp[2] * axise[0] + sin * axise[1];
        rotateZ[1] = temp[2] * axise[1] - sin * axise[0];
        rotateZ[2] = cos + temp[2] * axise[2];
        ZC_Vec4<float> columnZ = model[0] * rotateZ[0] + model[1] * rotateZ[1] + model[2] * rotateZ[2];

        model[0] = columnX;
        model[1] = columnY;
        model[2] = columnZ;
    }
};

struct ZC_TextSceneTurnedToCamera : public ZC_TextData, public ZC_TurnToCamera
{
    ZC_TextSceneTurnedToCamera(typename ZC_Fonts::NameHeight fontData, const std::string& _text, ZC_TextAlignment _alignment,
            const ZC_Vec3<float>& _position, float _scale = 0.f)
        : ZC_TextData(ZC_ShProgs::Get(ZC_ShProgs::Name::ZC_TextScene), FontOrigin::center, fontData, _text, _alignment, ZC_RendererLevels::TextScene),
        ZC_TurnToCamera(_position, _scale == 0.f ? 0.01f : _scale)
    {
        this->rsController.SetUniformsData(ZC_UN_unModel, &(this->model));
    }

    ZC_TextSceneTurnedToCamera MakeCopy()
    {
        return { *this };
    }

private:
    ZC_TextSceneTurnedToCamera(const ZC_TextSceneTurnedToCamera& tsttc)
        : ZC_TextData(dynamic_cast<const ZC_TextData&>(tsttc)),
        ZC_TurnToCamera(dynamic_cast<const ZC_TurnToCamera&>(tsttc))
    {
        this->rsController.SetUniformsData(ZC_UN_unModel, &(this->model));
    }
};

struct ZC_TextWindowIntoScene : public ZC_TextData
{
    ZC_TextWindowIntoScene(typename ZC_Fonts::NameHeight fontData, const std::string &_text, ZC_TextAlignment _alignment,
            const ZC_Vec3<float>& position)
        : ZC_TextData(ZC_ShProgs::Get(ZC_ShProgs::Name::ZC_TextWindowIntoScene), FontOrigin::bottomCenter, fontData, _text, _alignment, ZC_RendererLevels::TextWindowIntoScene),
        positionIntoScene(position)
    {
        rsController.SetUniformsData(ZC_UN_unPositionScene, positionIntoScene.Begin());
    }

    void SetPosition(const ZC_Vec3<float>& position) noexcept
    {
        positionIntoScene = position;
    }

    ZC_TextWindowIntoScene MakeCopy()
    {
        return { *this };
    }

private:
    ZC_Vec3<float> positionIntoScene;

    ZC_TextWindowIntoScene(const ZC_TextWindowIntoScene& twis)
        : ZC_TextData(dynamic_cast<const ZC_TextData&>(twis)),
        positionIntoScene(twis.positionIntoScene)
    {
        rsController.SetUniformsData(ZC_UN_unPositionScene, positionIntoScene.Begin());
    }
};

//              add support of users renderer levels in ZC_TextData, add Text in core
ZC_TextWindowIntoScene* pText;
void SetColor(float);

// void MouseMove(float x, float y, float xrel, float yrel, float time)
// {
//     ZC_cout("x = " + std::to_string(x) + "; [-1,1] = " + std::to_string(ZC_ToMinusPlusOneRange(x / 800.f))
//         + ";     y = " + std::to_string(y) + "; [-1,1] = " + std::to_string((1.f - (y / 600.f)) * 2 - 1.f));
// }

int ZC_main()
{
    using namespace ZC_Window;
    ZC_Window::MakeWindow(ZC_Window_Multisampling_4 | ZC_Window_Border, 800.f, 600.f, "ZeroCreator");
    // window->SetFPS(0);

    size_t textHeight = 100;
    ZC_Fonts::NameHeight fonts{ZC_FontName::Arial, textHeight};
    ZC_Fonts::Load(&fonts, 1);

    ZC_Window::GlClearColor(0.3f, 0.3f, 0.3f, 1.f);
    ZC_Window::GlEnablePointSize();
    
    ZCR_Scene scene;
    
    // static const ulong firstASCII = 32,
    //     lastASCII = 127;
    // char str[(lastASCII - firstASCII) * 2];
    // for (size_t i = 0, strIndex = 0; i < (lastASCII - firstASCII) * 2; i++, strIndex++)
    // {
    //     str[i] = firstASCII + strIndex;
    //     ++i;
    //     if (lastASCII - firstASCII == i)
    //         str[i] = '\n';
    //     else
    //         str[i] = ' ';
    // }
    std::string str = "T.O.P\np'opK_a\nD I M A";

    // ZC_TextScene text1({ZC_FontName::Arial, textHeight}, str, ZC_TextAlignment::Left);
    // text1.SetColor({1.f, 0.f, 0.f});
    // ZC_Vec3<float> text2Pos(0.f, 5.f, 0.f);
    // float text2RotAngle = 30.f;
    // ZC_Vec3<float> text2Rotaxises(0.f, 0.f, 1.f);
    
    // auto text2 = text1.MakeCopy(&text2Pos, &text2RotAngle, &text2Rotaxises, nullptr);
    // text2.SetColor({0.f, 0.f, 1.f});

    // ZC_TextWindow text({ZC_FontName::Arial, textHeight}, str, ZC_TA_Right, 0.f, 0.f, ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Top_Pixel);


    // ZC_TextSceneTurnedToCamera textSceneTurn({ZC_FontName::Arial, textHeight}, str, ZC_TextAlignment::Left, {20,20,10});
    // ZC_TextSceneTurnedToCamera textSceneTurn({ZC_FontName::Arial, textHeight}, str, ZC_TextAlignment::Left, {5.f, 0.f, 2.f});
    // ZC_TextSceneTurnedToCamera textSceneTurn({ZC_FontName::Arial, textHeight}, str, ZC_TextAlignment::ZC_TA_Right, {0.f, 0.f, 0.f});
    // auto textSceneTurn2 = textSceneTurn.MakeCopy();
    // textSceneTurn2.SetPosition({7.f, 0.f, 0.f});
    // pText = &textSceneTurn;

    ZC_TextWindowIntoScene textWIS({ZC_FontName::Arial, textHeight}, str, ZC_TextAlignment::ZC_TA_Center, {5.f, 3.f, 2.f});
    auto copy = textWIS.MakeCopy();
    copy.SetPosition({-4.f, -5.f, -3});


    pText = &copy;
    // auto text1 = text.MakeCopy();
    // text1.SetColor({0, 1.f, 0});
    // text1.SetIndentData(20.f, 40.f, ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Bottom_Pixel);

    ZC_Events::ConnectButtonDown(ZC_ButtonID::K_Q, {&SetColor});

    // ZC_Events::ConnectMouseMove({&MouseMove});

    ZC_Window::RuntMainCycle();
    
    return 0;
}

std::string first = "first\nLol\nsdaasdffffffafsdf\nsdaf",
    second = "{seconD}}\nasdf'_",
    current = first;


void SetColor(float)
{
    if (current == first)
    {
        // pText->NeedDraw(false);
        // pText->SetTextAndAlignment(second, ZC_TextAlignment::Left);
        // pText->SetColor({1.f, 1.f, 0.f});
        // pText->SetIndentData(0.2f, 90.f, ZC_WOIF__X_Left_Percent | ZC_WOIF__Y_Center);
        // pText->SetPosition({5.f, 0.f, 2.f});
        // pText->SetScale(0.001);
        // pText->SetAlignment(ZC_TA_Left);
        pText->SetColor({1.f, 0.f, 0.f});
        // pText->NeedDraw(true);
        current = second;
    }
    else
    {
        // pText->NeedDraw(true);
        // pText->SetTextAndAlignment(first, ZC_TextAlignment::Right);
        // pText->SetColor({0.f, 1.f, 1.f});
        // pText->SetIndentData(20.f, 40.f, ZC_WOIF__X_Right_Pixel | ZC_WOIF__Y_Top_Pixel);
        // pText->SetPosition({0.f, 0.f, 0.f});
        // pText->SetScale(0.01);
        // pText->SetAlignment(ZC_TA_Right);
        pText->SetColor({0.f, 1.f, 0.f});
        // pText->NeedDraw(false);
        current = first;
    }
}


//  pos = perspView * pos       -> with w divisor component
//  x = pos.x / pos.w; y = pos.y / pos.w        -> x, y [-1,1]
//  z = pos.z / pos.w       -> depth valid values [0,1] -> 0 closer, 1 further; if (> 1 and < 0) -> out of frustrum

//  pos = ortho * vec4      -> x, y [-1,1]
        


//     enum En
// {
//     null    = 0,
//     one     = 1 << 1,
//     two     = 1 << 2,
//     three   = 1 << 3,
//     four    = 1 << 4,
//     five    = 1 << 5,
//     six     = 1 << 6,
//     seven   = 1 << 7,
//     eight   = two | four,
// };
//     auto flags = En::one | En::eight | En::five;
//     if ((flags & En::three) == En::three)
//     {
//         int a = 3;
//     }

// class A
// {
// public:
//    A() = default;
//    explicit A(int _a) : a(_a){
//     ZC_cout("A ctr " + std::to_string(a));
//     }
//    A(const A& _a)
//     // = delete;
//     : a(_a.a)
//    {
//     ZC_cout("A() copy");
//     }
//    A& operator = (const A& _a)
//     // = delete;
//    {a = _a.a; 
//    ZC_cout("A = copy");
//     return *this;}
//    A(A&& _a)  noexcept : a(_a.a){_a.a = 0;
//    ZC_cout("A() move");
//    }
//    A& operator = (A&& _a) noexcept {a = _a.a; _a.a = 0;
//     ZC_cout("A = move");
//      return *this;}
//    virtual ~A(){
//     ZC_cout("A dtr " + std::to_string(a)); b = false;
//     }

//     bool operator < (const A& _a) const {return a < _a.a;}
//   //  virtual void Foo(){ZC_cout("A");}
//    int a = 0;
//    bool b = true;
// protected:
// private:
// };
// class B : public A
// {
// public:
//    B() = delete;
//    explicit B(int _a) : A(_a) {ZC_cout("B ctr");}
//    B(const B& _a) : A(dynamic_cast<const A&>(_a)){ZC_cout("B() copy");}
//    B& operator = (const B& _a){dynamic_cast<A&>(*this) = dynamic_cast<const A&>(_a); ZC_cout("B = copy"); return *this;}
//    B(B&& _a)  noexcept : A(dynamic_cast<A&&>(_a)){ZC_cout("B() move");}
//    B& operator = (B&& _a) noexcept {dynamic_cast<A&>(*this) = dynamic_cast<A&&>(_a); ZC_cout("B = move"); return *this;}
//    ~B() override{ZC_cout("B dtr");}
// };


        // //  Calculate a point on a 2D ellipse in the direction from the center of the ellipse to the corner.
        // ZC_Vec3<float> EllipsePoint(const ZC_Vec3<float>& ellipsesCenter, const ZC_Vec3<float>& ellipsX, const ZC_Vec3<float>& ellipsY, const ZC_Vec3<float>& corner)
        // {
        //     ZC_Vec3<float> direction = corner - ellipsesCenter;    //  direction of the desired point
        //     float radiusX = ZC_Vec::Length(ellipsX - ellipsesCenter),
        //         radiusY = ZC_Vec::Length(ellipsY - ellipsesCenter);
            
        //     //  if it's square, move the elipsCenter in the direction of the center by the length of radiusX or radiusY
        //     if (radiusX == radiusY) return ZC_Vec::MoveByLength(ellipsesCenter, direction, radiusX);

        //     //  ellips path https://ip76.ru/theory-and-practice/ellipse-point-coord/#eq_main
        //     //  work with triangle (ellipsCenter, center, ellipsX).
        //     float a = ZC_Vec::Length(corner - ellipsX),
        //         b = ZC_Vec::Length(corner - ellipsesCenter),  //  c - is radiusX
        //         cosA = ((b * b) + (radiusX * radiusX) - (a * a)) / (2.f * b * radiusX),   //  cosine theorema
        //         sinA = sqrt(1.f - (cosA * cosA)),   //  main trigonometric identity
        //         ellipsesAngle = atan2(radiusX * sinA, radiusY * cosA),  //  follow the link earlier, angleB
        //         x = radiusX * cos(ellipsesAngle),   //  x,y position of (center) on ellips
        //         y = radiusY * sin(ellipsesAngle),
        //         lengthXY = sqrt((x * x) + (y * y));
        //     return ZC_Vec::MoveByLength(ellipsesCenter, direction, lengthXY);
        // }
        
    //  упаковать цвет из float в uint
    // auto lambda = [](float r, float g, float a)
    // {
    //     return ((unsigned int)(r * 255.f) << 8 | (unsigned int)(g * 255.f)) << 8 | (unsigned int)(a * 255.f);
    // };
    //  распаковать из uint во float
    // unsigned int q = (r << 8 | g) << 8 | b;
    // float b = white << 24 >> 24,
    //     g = white << 16 >> 24,
    //     r = white >> 16;

//  упаковка r g b из диапазона (0, 255) в unsigned int
// #define Func(r, g, b){ ((unsigned int)r << 8 | g) << 8 | b }


    // //  texture loading
    // auto vectorOfTexturesCreator = pShPIS->texSets.GetCreator();
    // typedef typename ZC_TexSets::TextureName TexName;
    // for (auto pTextureName = vectorOfTexturesCreator.NextName(); pTextureName != nullptr; pTextureName = vectorOfTexturesCreator.NextName())
    // {
    //     switch (*pTextureName)
    //     {
    //     case TexName::texColor:
    //         vectorOfTexturesCreator.Add(ZC_Textures::LoadTexture2D(ZC_FSPath(ZCR_ZCRTexturePath).append("mesh.png").c_str())); break;
    //     }
    // }
    // std::vector<typename ZC_RSTexs::TexSet> texSets;
    // texSets.emplace_back(texSetName, vectorOfTexturesCreator.GetVector());

    // return { new ZC_RSTexs(pShPIS, std::move(vao), std::move(upDraw), std::move(buffers), std::move(texSets)) };




    // ZC_Sounds::LoadWAV("lp", "/home/dmitry/Загрузки/lp.wav");
    // ZC_upSound lp = ZC_Sounds::GetSound("lp");
    // ZC_Sounds::LoadWAV("Airplanes", "/home/dmitry/Загрузки/Airplanes.wav");
    // ZC_upSound airplanes = ZC_Sounds::GetSound("Airplanes");
    // ZC_Audio::OpenAudioStream(ZC_AudioSet(ZC_AudioSet::Channels::Stereo, 44100, ZC_AudioSet::BitsPerSample::S16));
    // lp->Play();
    // airplanes->Play();