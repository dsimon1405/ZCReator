#include "ZCR_ImGui.h"

#include <Objects/Figure/ZCR_Figures.h>

#include "imgui.h"

void ZCR_ImGui::ImGuiDraw()
{
        // ImGui::ShowDemoWindow();

    // MainWindow();
}

void ZCR_ImGui::MainWindow()
{
    using namespace ImGui;
    static ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar;
    static bool isWindowOpen;

    Begin("ZeroCReator1", NULL, windowFlags);

    static int pictureSize = 30;

    ImGuiIO& io = ImGui::GetIO();
    static const float w = 150.f, h = 50.f;
    SetWindowSize(ImVec2(w, h));
    SetWindowPos({20.f, 20.f});

    if (BeginTable("split", 4, 0, ImVec2(150, 0)))
    {
        TableNextColumn();
        if (ImageButton("Cube", reinterpret_cast<void*>(image.id), ImVec2(pictureSize, pictureSize), ImVec2(0.5f, 0.5f)))
        {
            ZCR_Figures::CreateFigure(ZCR_Figure::FigureName::FN_Cube);
        }
        
        TableNextColumn();
        if (ImageButton("Cube1", reinterpret_cast<void*>(image.id), ImVec2(pictureSize, pictureSize)))
        {
        }
        EndTable();
    }

    End();

    Begin("ZeroCReator", &isWindowOpen, windowFlags);

    static const float width = 300.f, height = 500.f;
    SetWindowSize(ImVec2(width, height));
    SetWindowPos({io.DisplaySize.x - width, 100.f});


    static bool isNeedCloseCreate = false;
    static bool isNeedOpenModel = false;
    static bool isNeedOpenEdit = false;

    SetOpenOrCloseStateOfCollapsingHeader(isNeedCloseCreate, false);
    if (CollapsingHeader("Create", ImGuiTreeNodeFlags_DefaultOpen))
    {

        if (BeginTable("split", 3, 0, ImVec2(150, 0)))
        {
            TableNextColumn();
            if (ImageButton("Cube", reinterpret_cast<void*>(image.id), ImVec2(pictureSize, pictureSize)))
            {
                ZCR_Figures::CreateFigure(ZCR_Figure::FigureName::FN_Cube);
                isNeedCloseCreate = true;
                isNeedOpenModel = true;
            }
            
            TableNextColumn();
            if (ImageButton("Cube1", reinterpret_cast<void*>(image.id), ImVec2(pictureSize, pictureSize)))
            {
                isNeedCloseCreate = true;
                isNeedOpenModel = true;
            }
            EndTable();
        }
    }
    
    SetOpenOrCloseStateOfCollapsingHeader(isNeedOpenModel, true);
    if (CollapsingHeader("Model", isNeedOpenModel))
    {
        if (BeginTable("split", 3, 0, ImVec2(150, 0)))
        {
            TableNextColumn(); ImageButton("Cube", reinterpret_cast<void*>(image.id), ImVec2(pictureSize, pictureSize));
            TableNextColumn(); ImageButton("Cube", reinterpret_cast<void*>(image.id), ImVec2(pictureSize, pictureSize));
            EndTable();
        }
    }
    
    SetOpenOrCloseStateOfCollapsingHeader(isNeedOpenEdit, true);
    if (CollapsingHeader("Model", isNeedOpenEdit))
    {
        if (BeginTable("split", 3, 0, ImVec2(150, 0)))
        {
            TableNextColumn(); ImageButton("Cube", reinterpret_cast<void*>(image.id), ImVec2(pictureSize, pictureSize));
            TableNextColumn(); ImageButton("Cube", reinterpret_cast<void*>(image.id), ImVec2(pictureSize, pictureSize));
            EndTable();
        }
    }

    End();
}

void ZCR_ImGui::SetOpenOrCloseStateOfCollapsingHeader(bool& rCurrentState, bool needOpen) noexcept
{
    if (rCurrentState)
    {
        rCurrentState = false;
        ImGui::SetNextItemOpen(needOpen);
    }
}

void F()
{
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
         
         bool a = true;
            bool show_another_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        

            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
            
            // ImGui::Window


            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            
            ImGui::Checkbox("Demo Window", &a);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            // ImGui::ImageButton("asf", (void*)image.id, ImVec2(50,50));
            if (ImGui::Button("Button", ImVec2(20,20)))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", io.DisplaySize.x, io.Framerate);
            ImGui::End();
}