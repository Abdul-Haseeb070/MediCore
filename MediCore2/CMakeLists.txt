cmake_minimum_required(VERSION 3.16)
project(MediCore LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/main.cpp"
)

add_executable(MediCore ${SOURCES})

target_include_directories(MediCore PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/src/utils"
    "${CMAKE_SOURCE_DIR}/src/exceptions"
    "${CMAKE_SOURCE_DIR}/src/core"
    "${CMAKE_SOURCE_DIR}/src/ui"
)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

target_link_libraries(MediCore PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

if(MSVC)
    target_compile_options(MediCore PRIVATE /W3)
else()
    target_compile_options(MediCore PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_custom_command(TARGET MediCore POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/data"
        "$<TARGET_FILE_DIR:MediCore>/data"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:MediCore>/assets"
    COMMENT "Copying data/ and assets/ to build output directory"
)
