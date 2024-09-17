cmake_minimum_required(VERSION 3.22)
project(ZCReator)

message(STATUS "======================= ZERO CREATOR START =======================")

#========================= compiler
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#========================= assets path
set(ZCR_ASSETS_DIRECTORY_NAME assets)
set(ZCR_ASSETS_DESTINATION_PATH 
    ${PROJECT_BINARY_DIR}/$<$<PLATFORM_ID:Windows>:$<IF:$<CONFIG:Debug>,Debug/,Release/>>${ZCR_ASSETS_DIRECTORY_NAME})   #   windows?

set(ZCR_MY_DEBUG 1)     # 1 true, 0 false   =>  use if(ZCR_MY_DEBUG EQUAL 1) - true; if(NOT ZCR_MY_DEBUG) - false

#========================= external libs
#   add submodules
if(NOT ZCR_MY_DEBUG)
    add_subdirectory(cmakeGitSubmodules)
endif()

set(ZCR_INCLUDE_DIRS)
set(ZCR_LINK_LIBS)

#   zeroCore
set(ZC_PC ON)
set(ZC_ANDROID OFF)
set(ZC_PARANT_BIN_DIR ${ZCR_ASSETS_DESTINATION_PATH})
set(ZC_AUDIO OFF)
set(ZC_IMGUI ON)
set(ZC_STATIC ON)
# set(ZC_STATIC OFF)
if(ZCR_MY_DEBUG EQUAL 1)
    set(ZC_MY_DEBUG 1)
    add_subdirectory(ZeroCore)
else()
    set(ZC_MY_DEBUG 0)
    add_subdirectory(external/ZeroCore)
endif()
set(ZCR_LINK_LIBS ${ZCR_LINK_LIBS} PUBLIC ZeroCore)

#========================= ZERO CREATOR =========================

#========================= sources
set(ZCR_SRC_FILES ${ZCR_SRC_FILES}
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/ZCR_Load.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Figure/ZCR_Figure.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Figure/ZCR_Figures.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Figure/ZCR_FigureCreator.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Figure/ZCR_ActiveFigures.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Figure/FigureData/ZCR_Color.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Figure/FigureData/ZCR_Point.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Figure/FigureData/ZCR_Line.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Figure/FigureData/ZCR_Surface.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Figure/FigureData/ZCR_VBO.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Figure/ImGui/ZCR_IGW_Figures.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Scene/ZCR_Scene.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Scene/ZCR_Mesh.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Scene/ZCR_Camera.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Scene/ImGui/ZCR_IGWBM_MainMenu.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Scene/ImGui/ZCR_IGWBM_Create.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Scene/ImGui/ZCR_IGWBM_Interface.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Scene/ImGui/ZCR_IGWBM_Mode.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Scene/ImGui/ZCR_IGWBM_M_Edit.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Scene/ImGui/ZCR_IGWBM_M_Model.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Scene/ImGui/ZCR_IGWBM_M_Sculpting.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Scene/GUI/ZCR_GUI_W_Scene.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Scene/GUI/ZCR_GUI_W_SceneOptions.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Scene/GUI/Mode/ZCR_GUI_SceneMode.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Scene/Orientation3D/ZCR_Orientation3D.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Scene/Orientation3D/ZCR_TextAxis.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Scene/Orientation3D/ZCR_TextAxises.cpp
    ${PROJECT_SOURCE_DIR}/src/Renderer/ZCR_DrawerCreator.cpp
    ${PROJECT_SOURCE_DIR}/src/Renderer/ZCR_ShaderKeeper.cpp
    )
    
add_executable(${PROJECT_NAME} ${ZCR_SRC_FILES})

#========================= include dirs
target_include_directories(${PROJECT_NAME} ${ZCR_INCLUDE_DIRS}
    PRIVATE ${PROJECT_SOURCE_DIR}/src
    # PUBLIC /home/dmitry/projects/external/glm/include   #   don't include
    )

#========================= link libs
target_link_libraries(${PROJECT_NAME} ${ZCR_LINK_LIBS})

#========================= compiler params
target_compile_options(${PROJECT_NAME}
    PRIVATE	$<IF:$<BOOL:${WIN32}>,-W3,-Wall>
            $<$<CONFIG:Release>:-O3>
    )

#========================= copy assets
add_custom_target(ZCR_COPY_ASSETS ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/${ZCR_ASSETS_DIRECTORY_NAME} ${ZCR_ASSETS_DESTINATION_PATH}
    COMMENT "Copy assets ${PROJECT_SOURCE_DIR}/${ZCR_ASSETS_DIRECTORY_NAME} => ${ZCR_ASSETS_DESTINATION_PATH}")
add_dependencies(${PROJECT_NAME} ZCR_COPY_ASSETS)


message(STATUS "======================== ZERO CREATOR END ========================")