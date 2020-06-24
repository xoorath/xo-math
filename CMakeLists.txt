project(xomath)

file(GLOB_RECURSE DEMO_SOURCES
    "source/*.h"
    "demo/*.cpp"
)

add_executable(${PROJECT_NAME} ${DEMO_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/source")