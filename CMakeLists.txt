cmake_minimum_required(VERSION 3.10)
project(MinecraftClone VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- OpenGL policy: prefer modern GLVND ---
cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)

# --- Dependencies ---
# Let users provide SFML_DIR on Windows if needed, otherwise search system paths
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

# --- Sources ---
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*.hpp)

# --- stb_image library ---
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${CMAKE_SOURCE_DIR}/stb)

# --- Core library ---
add_library(core ${SOURCES} ${HEADERS})
target_include_directories(core PUBLIC include)
target_link_libraries(core
    PUBLIC
        sfml-graphics
        sfml-window
        sfml-system
        GLEW::GLEW
        OpenGL::GL
        stb
    PRIVATE
        glm::glm
)

# --- Executable ---
add_executable(minecraft src/main.cpp)
target_link_libraries(minecraft PRIVATE core)

# --- Copy resources after build ---
add_custom_command(TARGET minecraft POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:minecraft>/res
)

add_custom_command(TARGET minecraft POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:minecraft>/shaders
)
