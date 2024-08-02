cmake_minimum_required(VERSION 3.10.2) 

project(voxels
    VERSION 0.0.1
    DESCRIPTION "Voxel engine"
    LANGUAGES CXX
)

# set(BUILD_SHARED_LIBS OFF)
# set(TRACY_ENABLE OFF)
# set(CMAKE_BUILD_TYPE Release)


include(CTest)
add_subdirectory(libs/GLFW)
add_subdirectory(libs/GLEW/build/cmake/)
add_subdirectory(libs/glm)
add_subdirectory(libs/tracy)
add_subdirectory(libs/googletest)
add_library(
    imgui 
    libs/imgui/imgui.cpp
    libs/imgui/imgui_draw.cpp
    libs/imgui/imgui_tables.cpp
    libs/imgui/imgui_widgets.cpp
    libs/imgui/imgui_demo.cpp
    libs/imgui/imconfig.h
    libs/imgui/imgui.h
    libs/imgui/imgui_internal.h
    libs/imgui/imstb_textedit.h
    libs/imgui/imstb_truetype.h
    libs/imgui/imgui_impl_opengl3.cpp
    libs/imgui/imgui_impl_opengl3.h
    libs/imgui/imgui_impl_glfw.cpp
    libs/imgui/imgui_impl_glfw.h
)


if (NOT CMAKE_BUILD_TYPE) 
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_library(cxx_setup INTERFACE) 
target_compile_options(cxx_setup  INTERFACE -Wpedantic -Wextra)
target_compile_features(cxx_setup INTERFACE cxx_std_20)
target_include_directories(cxx_setup INTERFACE "${PROJECT_SOURCE_DIR}/include/")
target_include_directories(cxx_setup INTERFACE "${PROJECT_SOURCE_DIR}/src/")
target_link_libraries(cxx_setup INTERFACE glfw glew glm imgui TracyClient)

add_subdirectory(src)
add_subdirectory(execs)
add_subdirectory(tests)
