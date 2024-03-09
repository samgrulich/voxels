cmake_minimum_required(VERSION 3.10.2) 

project(voxels)
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "src/*.hpp")
add_executable(${PROJECT_NAME} ${SOURCES})

# set(BUILD_SHARED_LIBS OFF)

add_subdirectory(libs/GLFW)
add_subdirectory(libs/GLEW/build/cmake/)
add_subdirectory(libs/glm)
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
    libs/imgui/backends/imgui_impl_opengl3.cpp
    libs/imgui/backends/imgui_impl_opengl3.h
    libs/imgui/backends/imgui_impl_glfw.cpp
    libs/imgui/backends/imgui_impl_glfw.h
)
#file(GLOB V_GLOB LIST_DIRECTORIES true "*")

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/include/")
target_link_libraries(${PROJECT_NAME} PUBLIC glfw glew glm imgui)

SET (CMAKE_CXX_FLAGS "-std=c++20 -g")


# add_subdirectory(src) # add directory with another CMakeLists

# install(TARGETS trim DESTINATION bin)


# link libraries
# target_include_directories(trim INTERFACE|PRIVATE|PUBLIC ${CMAKE_SOURCE_DIR}/include)
# target_link_directories(trim INTERFACE|PRIVATE|PUBLIC ${CMAKE_SOURCE_DIR}/lib)
# target_link_libraries(trim libname.a)
# target_compile_definitions

# for libs
# add_library(name STATIC|SHARED source1 source2 ... sourceN)


# find_library(varname libname NAME(S) libname1 libname2 ... libnameN DOC "docs for the lib :)" REQUIRED) # where libname1 are other possible names for the library
# if (${varname} NOT_FOUND) # or (${varname} STREQUAL "varname-NOTFOUND"})
#     message(FATAL_ERROR "Library not found")
# endif
