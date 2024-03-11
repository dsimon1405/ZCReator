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
#include <Ogl.h>

// #include <Objects/ImGui/ZCR_ImGui.h>
#include <Objects/Scene/ZCR_Scene.h>


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
        auto iter = std::find(vertices.begin(), vertices.end(), vertex);
        if (iter == vertices.end()) vertices.emplace_back(Vertex{vertex, {vData}});
        else iter->vdList.emplace_back(vData);
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

// class CharAtlas
// {
// public:
//     CharAtlas(const std::string& name, const unsigned& height);
//     static void LoadContent(const std::string& name, const unsigned& height);
//     void Update(const std::string& text, const unsigned& startX, const unsigned& startY, const ZC_Vec3<float>& color, const GLfloat& scale = 1.0f);
//     static void Draw();

// private:
//     struct Character
//     {
//         float advX; 	// advance.x
//         float advY;	    // advance.y

//         float bitmapW;	    // bitmap.width;
//         float bitmapH;  	// bitmap.height;

//         float bitmapLeft;	// bitmap_left;
//         float bitmapTop;	// bitmap_top;

//         float texX;	    // x offset of glyph in texture coordinates
//         float texY;	    // y offset of glyph in texture coordinates
//     };

//     struct Point
//     {
//         GLfloat x,
//             y,
//             s,  // ??? X
//             t;  // ??? Y
//     };

//     struct Text
//     {
//         ZC_Vec3<float> color;
//         ZC_Texture* texture;
//         std::vector<Point> coords;
//         static inline size_t coordsTotalSize;

//         bool operator<(const Text& text)
//         {
//             if (color != text.color)
//             {
//                 if (color[0] < text.color[0]) return true;
//                 if (color[0] > text.color[0]) return false;
//                 if (color[1] < text.color[1]) return true;
//                 if (color[1] > text.color[1]) return false;
//                 if (color[2] < text.color[2]) return true;
//             }
//             if (texture < text.texture) return true;
//             return false;
//         }
//     };

//     struct DrawingSet
//     {
//         const size_t startIndex;
//         size_t amount;
//         ZC_Vec3<float>* color;
//         ZC_Texture* texture;
//     };

//     ZC_Texture* texture;
//     static inline std::map<std::string, const std::shared_ptr<std::map<char, const Character>>> allCharacters;
//     std::shared_ptr<std::map<char, const Character>> characters;
//     static inline std::vector<Text> texts;
    
//     static void CalculateTextureSize(unsigned& texW, unsigned& texH, const FT_Face& face, const unsigned& firstASCII, const unsigned& lastASCII, const unsigned& borderPixel, const unsigned& texMaxW);
//     static std::shared_ptr<std::map<char, const Character>> FillTexture(const unsigned& texW, const unsigned& texH, const FT_Face& face, const unsigned& firstASCII, const unsigned& lastASCII, const unsigned& borderPixel, const unsigned& texMaxW);
//     const std::vector<Point> FillCoords(const std::string& text, GLfloat startX, GLfloat startY, const GLfloat& scale);
//     static std::vector<Point> PrepareDrawContent(std::list<DrawingSet>& drawingSets);
//     static bool InitVAO(const ZC_VAO& vao, const ZC_Buffer& vbo);
// };

struct ZC_ChACharacter
{
    float advX; 	// advance.x
    float advY;	    // advance.y

    float bitmapW;	    // bitmap.width;
    float bitmapH;  	// bitmap.height;

    float bitmapLeft;	// bitmap_left;
    float bitmapTop;	// bitmap_top;

    float texX;	    // x offset of glyph in texture coordinates
    float texY;	    // y offset of glyph in texture coordinates
};

#include <ft2build.h>
#include FT_FREETYPE_H
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

// CharAtlas::CharAtlas(const std::string& name, const unsigned& height)
// {
//     std::string fullName = name + std::to_string(height);
//     if (allCharacters.find(fullName) == allCharacters.end()) LoadContent(name, height);
//     texture = Texture2D::GetTexture(fullName);
//     characters = allCharacters[fullName];
// }

// void CharAtlas::LoadContent(const std::string& name, const unsigned& height)
// {
//     std::string fullName = name + std::to_string(height);
//     if (allCharacters.find(fullName) != allCharacters.end()) ZC_ErrorLogger::Err("CharAtlas --> " + fullName + " <-- already exist!", __FILE__, __LINE__);

//     FT_Library ft;
//     if (FT_Init_FreeType(&ft)) ZC_ErrorLogger::Err("ERROR::FREETYPE: Could not init FreeType Library!", __FILE__, __LINE__);

//     std::string fontPath = JSONReader::ReadKeyValues(ResourceManager::pathsToResources, std::vector<std::string>{"fonts", name})[0];
//     FT_Face face;
//     if (FT_New_Face(ft, ResourceManager::MakeFullPath(fontPath).c_str(), 0, &face))
//     {
//         FT_Done_FreeType(ft);
//         throw Exception("ERROR::FREETYPE: Failed to load font!", __FILE__, __LINE__);
//     }

//     FT_Set_Pixel_Sizes(face, 0, height);

//     const unsigned texMaxW = 1024,
//         borderPixel = 1,
//         firstASCII = 32,
//         lastASCII = 127;
//     unsigned texW = 0,
//         texH = 0;
//     FT_GlyphSlot glyph = face->glyph;

//     CalculateTextureSize(texW, texH, face, firstASCII, lastASCII, borderPixel, texMaxW);

//     glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

//     Texture2D* texture = Texture2D::Create(fullName, texW, texH, nullptr, GL_RED, true, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
//     texture->Bind();
//     std::shared_ptr<std::map<char, const Character>> characters = FillTexture(texW, texH, face, firstASCII, lastASCII, borderPixel, texMaxW);
//     texture->Unbind();
//     allCharacters.emplace(fullName, characters);

//     FT_Done_Face(face);
//     FT_Done_FreeType(ft);
// }

// void CharAtlas::Update(const std::string& text, const unsigned& startX, const unsigned& startY, const glm::vec3& color, const GLfloat& scale)
// {
//     if (text == "" || count(text.begin(), text.end(), ' ') == text.size()) return;

//     texts.emplace_back(Text{ color, texture, FillCoords(text, (GLfloat)startX, (GLfloat)startY, scale) });
// }

// const std::vector<CharAtlas::Point> CharAtlas::FillCoords(const std::string& text, GLfloat startX, GLfloat startY, const GLfloat& scale)
// {
//     std::vector<Point> coords;
//     const unsigned vertices = 6;
//     coords.reserve((text.length() - count(text.begin(), text.end(), ' ')) * vertices);

//     for (const unsigned char& c : text)
//     {
//         const Character& ch = (*characters)[c];
//         float x = startX + ch.bitmapLeft * scale,
//             y = startY + (ch.bitmapH - (ch.bitmapH - ch.bitmapTop)) * scale,
//             w = ch.bitmapW * scale,
//             h = ch.bitmapH * scale;

//         startX += ch.advX * scale;
//         //startY += ch.advY * scale;

//         if (!w || !h) continue;   //  ïðîïóñêàåì ïðîáåë

//         const float texWidth = (float)texture->width,
//             texHeight = (float)texture->height;

//         coords.emplace_back(Point{ x, y, ch.texX, ch.texY });                                                            //  ëåâûé âåðõíèé
//         coords.emplace_back(Point{ x, y - h, ch.texX, ch.texY + ch.bitmapH / texHeight });                               //  ëåâûé íèæíèé
//         coords.emplace_back(Point{ x + w, y - h, ch.texX + ch.bitmapW / texWidth, ch.texY + ch.bitmapH / texHeight });   //  ïðàâûé íèæíèé
//         coords.emplace_back(Point{ x, y, ch.texX, ch.texY });                                                            //  ëåâûé âåðõíèé
//         coords.emplace_back(Point{ x + w, y - h, ch.texX + ch.bitmapW / texWidth, ch.texY + ch.bitmapH / texHeight });   //  ïðàâûé íèæíèé  
//         coords.emplace_back(Point{ x + w, y, ch.texX + ch.bitmapW / texWidth, ch.texY });                                //  ïðàâûé âåðõíèé
//     }

//     Text::coordsTotalSize += coords.size();
//     return coords;
// }

// void CharAtlas::Draw()
// {
//     if (texts.size() == 0) return;

//     std::list<DrawingSet> drawingSets;
//     std::vector<Point> allCoords = PrepareDrawContent(drawingSets);

//     glEnable(GL_BLEND);
//     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//     static Shader* shader = Shader::GetShader("charAtlas");
//     static VAO vao;
//     static VBO vbo;
//     static bool itin = InitVAO(vao, vbo);

//     shader->Use();
//     vao.Bind();
//     vbo.Bind();

//     glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * Text::coordsTotalSize, &allCoords[0], GL_DYNAMIC_DRAW);
//     for (DrawingSet& drawingSet : drawingSets)
//     {
//         if (drawingSet.color) shader->SetVec3("textColor", *drawingSet.color);
//         if (drawingSet.texture) drawingSet.texture->Bind();
//         glDrawArrays(GL_TRIANGLES, drawingSet.startIndex, drawingSet.amount);
//     }

//     vao.Unbind();
//     vbo.Unbind();

//     glDisable(GL_BLEND);

//     texts.clear();
//     Text::coordsTotalSize = 0;
// }

// void CharAtlas::CalculateTextureSize(unsigned& texW, unsigned& texH, const FT_Face& face, const unsigned& firstASCII, const unsigned& lastASCII, const unsigned& borderPixel, const unsigned& texMaxW)
// {
//     unsigned rowW = 0,
//         rowH = 0;
//     FT_GlyphSlot glyph = face->glyph;

//     for (auto symbol = firstASCII; symbol < lastASCII; symbol++)
//     {
//         if (FT_Load_Char(face, symbol, FT_LOAD_RENDER))
//         {
//             std::cout << "ERROR::FREETYTPE: Failed to load Glyph\n";
//             continue;
//         }

//         //FT_Render_Glyph(glyph, FT_RENDER_MODE_SDF); // <-- And this is new

//         if (rowW + glyph->bitmap.width + borderPixel > texMaxW)
//         {
//             texW = std::max(rowW, texW);
//             texH += rowH + borderPixel;
//             rowW = 0;
//             rowH = 0;
//         }

//         rowW += glyph->bitmap.width + borderPixel;
//         rowH = std::max(rowH, glyph->bitmap.rows);
//     }

//     texW = std::max(texW, rowW);
//     texH += rowH + borderPixel;
// }

// std::shared_ptr<std::map<char, const CharAtlas::Character>> CharAtlas::FillTexture(const unsigned& texW, const unsigned& texH, const FT_Face& face, const unsigned& firstASCII, const unsigned& lastASCII, const unsigned& borderPixel, const unsigned& texMaxW)
// {
//     float texX = 0,
//         texY = 0;
//     unsigned rowH = 0;
//     FT_GlyphSlot glyph = face->glyph;

//     std::shared_ptr<std::map<char, const Character>> characters = std::make_shared<std::map<char, const Character>>();

//     for (auto symbol = firstASCII; symbol < lastASCII; symbol++)
//     {
//         if (FT_Load_Char(face, symbol, FT_LOAD_RENDER)) //      ?????????????
//         {
//             std::cout << "ERROR::FREETYTPE: Failed to load Glyph\n";
//             continue;
//         }

//         //FT_Render_Glyph(glyph, FT_RENDER_MODE_SDF); // <-- And this is new

//         if (texX + glyph->bitmap.width + borderPixel > texMaxW)
//         {
//             texY += rowH + borderPixel;
//             rowH = 0;
//             texX = 0;
//         }

//         if (glyph->bitmap.width != 0 && glyph->bitmap.rows != 0)
//             glTexSubImage2D(GL_TEXTURE_2D, 0, texX, texY, glyph->bitmap.width, glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);

//         Character character;
//         character.advX = glyph->advance.x >> 6;
//         character.advY = glyph->advance.y >> 6;
//         character.bitmapW = glyph->bitmap.width;
//         character.bitmapH = glyph->bitmap.rows;
//         character.bitmapLeft = glyph->bitmap_left;
//         character.bitmapTop = glyph->bitmap_top;
//         character.texX = texX / (float)texW;
//         character.texY = texY / (float)texH;

//         characters->emplace(symbol, character);

//         texX += glyph->bitmap.width + borderPixel;
//         rowH = std::max(glyph->bitmap.rows, rowH);
//     }

//     return characters;
// }

// std::vector<CharAtlas::Point> CharAtlas::PrepareDrawContent(std::list<DrawingSet>& drawingSets)
// {
//     std::sort(texts.begin(), texts.end());

//     std::vector<Point> allCoords;
//     allCoords.reserve(Text::coordsTotalSize);
//     for (auto text = texts.begin(); text != texts.end(); text++)
//     {
//         allCoords.insert(allCoords.begin() + allCoords.size(), text->coords.begin(), text->coords.end());
//         if (drawingSets.size() == 0)
//         {
//             drawingSets.emplace_back(DrawingSet{ 0, text->coords.size(), &text->color, text->texture });
//             continue;
//         }
//         auto previousText = text - 1;
//         DrawingSet& previousSet = drawingSets.back();
//         if (previousText->color == text->color && previousText->texture == text->texture)
//         {
//             previousSet.amount += text->coords.size();
//         }
//         else
//         {
//             drawingSets.emplace_back(DrawingSet
//                 {
//                     previousSet.startIndex + previousSet.amount,
//                     text->coords.size(),
//                     previousText->color == text->color ? nullptr : &text->color,
//                     previousText->texture == text->texture ? nullptr : text->texture
//                 });
//         }
//     }

//     return allCoords;
// }

// bool CharAtlas::InitVAO(const VAO& vao, const VBO& vbo)
// {
//     vao.Bind();
//     vbo.Bind();
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
//     vao.Unbind();
//     vbo.Unbind();
//     return true;
// }


#include <ZC/File/ZC_File.h>
struct ZC_Fonts
{
    enum Name
    {
        Arial,
    };

    struct NameHeight
    {
        Name name;
        size_t height;

        bool operator < (const NameHeight& fs) const noexcept
        {
            return name < fs.name || height < fs.height;    //  && || ???
        }
    };

    struct Font
    {
        struct Point
        {
            float x,
                y,
                s,  // texCoords X
                t;  // texCoords Y
        };

        ZC_Texture texture;
        std::map<char, ZC_ChACharacter> characters;
        float width,
            height;

        const std::vector<Point> FillCoords(const std::string& text, GLfloat startX, GLfloat startY, const GLfloat& scale)
        {
            std::vector<Point> coords;
            const unsigned vertices = 6;
            coords.reserve((text.length() - count(text.begin(), text.end(), ' ')) * vertices);

            for (const char& c : text)
            {
                const ZC_ChACharacter& ch = characters[c];
                float x = startX + ch.bitmapLeft * scale,
                    y = startY + (ch.bitmapH - (ch.bitmapH - ch.bitmapTop)) * scale,
                    w = ch.bitmapW * scale,
                    h = ch.bitmapH * scale;

                startX += ch.advX * scale;
                //startY += ch.advY * scale;

                if (!w || !h) continue;   //  ïðîïóñêàåì ïðîáåë

                const float texWidth = width,
                    texHeight = height;

                coords.emplace_back(Point{ x, y, ch.texX, ch.texY });                                                            //  ëåâûé âåðõíèé
                coords.emplace_back(Point{ x, y - h, ch.texX, ch.texY + ch.bitmapH / texHeight });                               //  ëåâûé íèæíèé
                coords.emplace_back(Point{ x + w, y - h, ch.texX + ch.bitmapW / texWidth, ch.texY + ch.bitmapH / texHeight });   //  ïðàâûé íèæíèé
                coords.emplace_back(Point{ x, y, ch.texX, ch.texY });                                                            //  ëåâûé âåðõíèé
                coords.emplace_back(Point{ x + w, y - h, ch.texX + ch.bitmapW / texWidth, ch.texY + ch.bitmapH / texHeight });   //  ïðàâûé íèæíèé  
                coords.emplace_back(Point{ x + w, y, ch.texX + ch.bitmapW / texWidth, ch.texY });                                //  ïðàâûé âåðõíèé
            }

            return coords;
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

    static std::string GetPath(Name name)
    {
        static const ZC_FSPath ZC_fontsPath = ZC_FSPath(ZC_ZCDirPath).append("fonts");
        switch (name)
        {
        case Arial: return ZC_FSPath(ZC_fontsPath).append("arial.ttf").string();
        default: return "";
        }
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

            //FT_Render_Glyph(glyph, FT_RENDER_MODE_SDF); // <-- And this is new

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

        std::map<char, ZC_ChACharacter> characters;

        for (auto symbol = firstASCII; symbol < lastASCII; ++symbol)
        {
            if (FT_Load_Char(face, symbol, FT_LOAD_RENDER)) continue;

            //FT_Render_Glyph(glyph, FT_RENDER_MODE_SDF); // <-- And this is new

            if (texX + glyph->bitmap.width + borderPixel > texMaxW)
            {
                texY += rowH + borderPixel;
                rowH = 0;
                texX = 0;
            }

            if (glyph->bitmap.width != 0 && glyph->bitmap.rows != 0)
                glTexSubImage2D(GL_TEXTURE_2D, 0, texX, texY, glyph->bitmap.width, glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);

            ZC_ChACharacter character;
            character.advX = glyph->advance.x >> 6;
            character.advY = glyph->advance.y >> 6;
            character.bitmapW = glyph->bitmap.width;
            character.bitmapH = glyph->bitmap.rows;
            character.bitmapLeft = glyph->bitmap_left;
            character.bitmapTop = glyph->bitmap_top;
            character.texX = texX / (float)texW;
            character.texY = texY / (float)texH;

            characters.emplace(symbol, character);

            texX += glyph->bitmap.width + borderPixel;
            rowH = std::max(glyph->bitmap.rows, rowH);
        }
        texture.Unbind();

        return { std::move(texture), std::move(characters), static_cast<float>(texW), static_cast<float>(texH) };
    }
};

struct ZC_RSText : public ZC_RS
{
    ZC_Texture* pTexture;

    ZC_RSText(typename ZC_ShProgs::ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_GLDraw>&& _upDraw,
            std::forward_list<ZC_Buffer>&& _buffers, ZC_Texture* _pTexture);
    ZC_RSText(ZC_RSText&& rs);

    void Draw(Level lvl) override;

    /*
    Return unique pointer to ZC_RendererSetAndDrawingSet.

    texSetName - name of the texture set (ZC_RSTexs::TexSet), can be nullptr if used for heir ZC_RSNonTex.
    stencilScale - scale for drawing the stencil border of the object. Should be greater than 1.0f. Can be anything if ZC_RendererSet::Level::Stencil will not be used.
    stencilColor - color of stencil border packed in unsigned int. Packing -> unsigned char[32] -> [0 - 7]{no metter}, [7 - 15]{red}, [15 - 23]{green}, [23 - 31]{blue}
    */
    ZC_uptr<ZC_RendererSetAndDrawingSet> Make_uptrRendererSetDrawingSet(const char* texSetName, float stencilScale, unsigned int stencilColor) override;

    /*
    Return shared pointer to ZC_RendererSetAndDrawingSet.

    texSetName - name of the texture set (ZC_RSTexs::TexSet), can be nullptr if used for heir ZC_RSNonTex.
    stencilScale - scale for drawing the stencil border of the object. Should be greater than 1.0f. Can be anything if ZC_RendererSet::Level::Stencil will not be used.
    stencilColor - color of stencil border packed in unsigned int. Packing -> unsigned char[32] -> [0 - 7]{no metter}, [7 - 15]{red}, [15 - 23]{green}, [23 - 31]{blue}
    */
    ZC_sptr<ZC_RendererSetAndDrawingSet> Make_sptrRendererSetDrawingSet(const char* texSetName, float stencilScale, unsigned int stencilColor) override;
    void Add(DrawingSet* pDS) override;
    void Erase(DrawingSet* pDS) override;
    
private:
    LevelController levelController;
};

ZC_RSText::ZC_RSText(typename ZC_ShProgs::ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_GLDraw>&& _upDraw,
        std::forward_list<ZC_Buffer>&& _buffers, ZC_Texture* _pTexture)
    : ZC_RS(pShPInitSet, std::move(_vao), std::move(_upDraw), std::move(_buffers)),
    pTexture(_pTexture)
{}

ZC_RSText::ZC_RSText(ZC_RSText&& rs)
    : ZC_RS(dynamic_cast<ZC_RS&&>(rs))
{}

void ZC_RSText::Draw(Level lvl)
{
    vao.BindVertexArray();
    levelController.Draw(lvl, upGLDraw, pTexture, 1);
}

ZC_uptr<ZC_RendererSetAndDrawingSet> ZC_RSText::Make_uptrRendererSetDrawingSet(const char* texSetName, float stencilScale, unsigned int stencilColor)
{
    return { new ZC_RendererSetAndDrawingSet(this, { pBaseUniforms->GetCopy(), Level::None, nullptr, stencilScale, stencilColor }) };
}

ZC_sptr<ZC_RendererSetAndDrawingSet> ZC_RSText::Make_sptrRendererSetDrawingSet(const char* texSetName, float stencilScale, unsigned int stencilColor)
{
    return { new ZC_RendererSetAndDrawingSet(this, { pBaseUniforms->GetCopy(), Level::None, nullptr, stencilScale, stencilColor }) };
}

void ZC_RSText::Add(DrawingSet* pDS)
{
    if (levelController.Add(pDS)) AddToRenderer(pDS->lvl);
}

void ZC_RSText::Erase(DrawingSet* pDS)
{
    if (levelController.Erase(pDS)) EraseFromRenderer(pDS->lvl);
}

struct ZC_TextWindow
{
    typedef typename ZC_Fonts::Font Font;
    Font* pFont;
    ZC_uptr<ZC_RS> upRS;
    ZC_uptr<ZC_RendererSetAndDrawingSet> upRSADS;

    ZC_TextWindow(typename ZC_Fonts::NameHeight nameHeight, const std::string& text, const ZC_Vec3<float>& color)
        : pFont(ZC_Fonts::GetFont(nameHeight)),
        upRS(MakeRS(text)),
        upRSADS(upRS->Make_uptrRendererSetDrawingSet("", 0, 0))
    {
        uint colorUint = ZC_PackColorFloatToUInt(color[0], color[1], color[2]);
        upRSADS->SetUniformsData(ZC_Uniform::Name::unColor, &colorUint);

        upRSADS->SwitchToLvl(ZC_RendererSet::Level::Drawing);
    }

    ZC_uptr<ZC_RS> MakeRS(const std::string& text)   //  symbolsInString exclude whitespaces
    {
        ZC_Buffer vbo(GL_ARRAY_BUFFER);
        auto pointsCoords = pFont->FillCoords(text, 300, 300, 1.f);
        vbo.BufferData(pointsCoords.size() * sizeof(ZC_Fonts::Font::Point), &(pointsCoords[0]), GL_DYNAMIC_DRAW);   //  add check on dynamic or static draw

        auto upDraw = ZC_uptrMakeFromChild<ZC_GLDraw, ZC_DrawArrays>(GL_TRIANGLES, 0, pointsCoords.size() * 6);   //  change on FAN or STRIP

        typename ZC_ShProgs::ShPInitSet* pShPIS = ZC_ShProgs::Get(ZC_ShProgs::Name::ZC_TextWindow);

        ZC_VAO vao;
        vao.Config(pShPIS->vaoConData, vbo, nullptr, 0, 0);

        std::forward_list<ZC_Buffer> buffers;
        buffers.emplace_front(std::move(vbo));

        return { new ZC_RSText(pShPIS, std::move(vao), std::move(upDraw), std::move(buffers), &(pFont->texture)) };
    }
};

struct ZC_TextScene
{

};
#include <ZC/Video/OpenGL/Renderer/ZC_RSTexs.h>
int ZC_main()
{
    using namespace ZC_Window;
    ZC_Window::MakeWindow(ZC_Window_Multisampling_4 | ZC_Window_Border, 800.f, 600.f, "ZeroCreator");
    // window->SetFPS(0);
    ZC_Fonts::NameHeight fonts[]{ZC_Fonts::Name::Arial, 200};
    ZC_Fonts::Load(fonts, 1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    ZC_Window::GlClearColor(0.3f, 0.3f, 0.3f, 1.f);
    ZC_Window::GlEnablePointSize();
    
    ZCR_Scene scene;
    ZC_TextWindow text({ZC_Fonts::Name::Arial, 200}, "lolka", {1.f, 0.f, 0.f});

    ZC_Window::RuntMainCycle();
    
    return 0;
}
        

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