file(GLOB_RECURSE SOURCES "./*.cpp" "./*.h" "./*.hpp")

add_library(tools ${SOURCES})
target_link_libraries(tools PUBLIC cxx_setup)
