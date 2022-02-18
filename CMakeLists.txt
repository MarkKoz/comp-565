cmake_minimum_required(VERSION 3.15)
project(cs_565_01 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Needed by Clang tooling (clang-tidy).

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW lib only")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")

add_subdirectory(lib/glad)
add_subdirectory(lib/glfw)

add_executable(${PROJECT_NAME}
    src/io.c
    src/main.c
    src/shader.c
    src/vertex.c
    src/window.c)
target_include_directories(${PROJECT_NAME} PUBLIC
    include)

target_link_libraries(${PROJECT_NAME} PUBLIC glad)
target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENGL_LIBRARIES} glfw)

if (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    target_link_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:/DEBUG:FASTLINK>")

    # Hide the console window when the program launches.
    list(APPEND FLAGS_LINK_NO_CONSOLE
        /SUBSYSTEM:WINDOWS
        /ENTRY:mainCRTStartup)

    list(APPEND FLAGS_WARN
        /W4)
    list(APPEND FLAGS_DEBUG
        /Od)
    list(APPEND FLAGS_RELEASE
        /O2)
else ()
    # Hide the console window when the program launches.
    if (MSYS OR MINGW)
        list(APPEND FLAGS_LINK_NO_CONSOLE
            -mwindows)
    endif ()

    list(APPEND FLAGS_WARN
        -Wall
        -Wcast-align
        -Wconversion
        -Wextra
        -Wfloat-equal
        -Wlogical-op
        -Wmissing-declarations
        -Wpedantic
        -Wredundant-decls
        -Wshadow
        -Wundef)
    list(APPEND FLAGS_DEBUG
        -g3
        -ggdb)
    list(APPEND FLAGS_RELEASE
        -DNDEBUG
        -O2)
endif ()

list(APPEND FLAGS_COMMON
    -DGLFW_INCLUDE_NONE)

target_link_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:RELEASE>:${FLAGS_LINK_NO_CONSOLE}>")
target_compile_options(${PROJECT_NAME} PUBLIC "${FLAGS_WARN}")
target_compile_options(${PROJECT_NAME} PUBLIC "${FLAGS_COMMON}")
target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:${FLAGS_DEBUG}>")
target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:RELEASE>:${FLAGS_RELEASE}>")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME output)

install(TARGETS ${PROJECT_NAME} RUNTIME)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/resources/ DESTINATION resources)
