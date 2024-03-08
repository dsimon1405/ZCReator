cmake_minimum_required(VERSION 3.22)
project(ZCReator)

message(STATUS "======================= ZERO CREATOR START =======================")

#========================= compiler
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#========================= assets path
set(ZCR_ASSETS_DIRECTORY_NAME assets)
set(ZCR_ASSETS_DESTINATION_PATH 
    ${PROJECT_BINARY_DIR}/$<$<PLATFORM_ID:Windows>:$<IF:$<CONFIG:Debug>,Debug,Release>>/${ZCR_ASSETS_DIRECTORY_NAME})   #   windows?

#========================= external libs
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
add_subdirectory(external/ZeroCore)
set(ZCR_LINK_LIBS ${ZCR_LINK_LIBS} PUBLIC ZeroCore)

#========================= ZERO CREATOR =========================

#========================= sources
set(ZCR_SRC_FILES ${ZCR_SRC_FILES}
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Figure/ZCR_Figure.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Figure/ZCR_Figures.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Figure/ZCR_ActiveFigures.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Figure/FigureSet/ZCR_Color.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Figure/FigureSet/ZCR_FigureData.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Figure/FigureSet/ZCR_FigureSet.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Figure/FigureSet/ZCR_Point.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Figure/FigureSet/ZCR_Line.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Figure/FigureSet/ZCR_Surface.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Figure/FigureSet/ZCR_VBO.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Scene/ZCR_Scene.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Scene/ZCR_Mesh.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Scene/ZCR_Camera.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Scene/ZCR_Orientation3D.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Scene/ImGui/ZCR_IGWCreateAndMode.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/Scene/ImGui/ZCR_IGWFigures.cpp
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

# #========================= copy assets

add_custom_target(ZCR_COPY_ASSETS ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/${ZCR_ASSETS_DIRECTORY_NAME} ${ZCR_ASSETS_DESTINATION_PATH}
    COMMENT "Copy assets")
add_dependencies(${PROJECT_NAME} ZCR_COPY_ASSETS)


message(STATUS "======================== ZERO CREATOR END ========================")
