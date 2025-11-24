cmake_minimum_required(VERSION 3.26)
project(IntelligenceQuest LANGUAGES C CXX)

#####################################################################
# 1. Build‑system & compiler basics
#####################################################################
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

# Prefer Ninja when the user hasn’t chosen a generator explicitly
if(NOT CMAKE_GENERATOR)
    set(CMAKE_GENERATOR "Ninja" CACHE INTERNAL "" FORCE)
endif()

#####################################################################
# 2. Third‑party dependencies
#####################################################################

include(FetchContent)

# 2‑A  GLFW — build from upstream source (tests/docs/examples disabled)
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.4
)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)           # exposes target: glfw

# 2‑B  GLAD — compile the single translation unit & expose headers
add_library(glad STATIC
        include/glad/src/glad.c
)
target_include_directories(glad PUBLIC
        include/glad/include
)

# 2‑C  GLM — pure header library
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE
        include/glm
)

# 2‑D  miniaudio — single‑header library
add_library(miniaudio INTERFACE)
target_include_directories(miniaudio INTERFACE
        include/miniaudio
)

# 2‑E  nlohmann/json — single‑header library
add_library(nlohmann INTERFACE)
target_include_directories(nlohmann INTERFACE
        include/nlohmann
)

# 2‑F  stb_image — single‑header library
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE
        include/stb_image
)

#####################################################################
# 3. Your own code
#####################################################################
# The sub‑directory must create an executable or library target.
add_subdirectory(src)  # expected to define target: IntelligenceQuest

# Link third‑party libraries to the main target if it exists.
if(TARGET IntelligenceQuest)
    target_link_libraries(IntelligenceQuest PRIVATE
            glfw
            glad
            glm
            miniaudio
            nlohmann
            stb_image
    )
endif()
