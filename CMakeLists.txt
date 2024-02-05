cmake_minimum_required(VERSION 3.22)
project(ZCReator)

message(STATUS "======================= ZERO CREATOR START =======================")

#========================= compiler
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#========================= external libs
set(ZCR_INCLUDE_DIRS)
set(ZCR_LINK_LIBS)

#   zeroCore
set(ZC_PC ON)
set(ZC_ANDROID OFF)
set(ZC_PARANT_BIN_DIR ${PROJECT_BINARY_DIR})
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
    ${PROJECT_SOURCE_DIR}/src/Objects/ZCR_Cube.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/ZCR_Figure.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/ZCR_Line.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/ZCR_Point.cpp
    ${PROJECT_SOURCE_DIR}/src/Objects/ZCR_Triangle.cpp
    )

add_executable(${PROJECT_NAME} ${ZCR_SRC_FILES})

#========================= include dirs
target_include_directories(${PROJECT_NAME} ${ZCR_INCLUDE_DIRS}
    PRIVATE ${PROJECT_SOURCE_DIR}/src
    # PUBLIC /home/dmitry/projects/external/glm/include
    )

#========================= link libs
target_link_libraries(${PROJECT_NAME} ${ZCR_LINK_LIBS})

#========================= compiler params
target_compile_options(${PROJECT_NAME}
    PRIVATE	$<IF:$<BOOL:${WIN32}>,-W3,-Wall>
            $<$<CONFIG:Release>:-O3>)

# #========================= copy assets
# set(ZCR_ASSETS_DIRECTORY_NAME assets)
# set(ZCR_ASSETS_DESTINATION_PATH 
#     $<$<BOOL:${SK_PC}>:${PROJECT_BINARY_DIR}/$<$<PLATFORM_ID:Windows>:$<IF:$<CONFIG:Debug>,Debug,Release>>/${ZCR_ASSETS_DIRECTORY_NAME}>)   #   window?

# add_custom_target(ZCR_COPY_ASSETS ALL
#     COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/${ZCR_ASSETS_DIRECTORY_NAME} ${ZCR_ASSETS_DESTINATION_PATH}
#     COMMENT "Copy assets")
# add_dependencies(${PROJECT_NAME} ZCR_COPY_ASSETS)


message(STATUS "======================== ZERO CREATOR END ========================")
