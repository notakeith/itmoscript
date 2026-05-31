cmake_minimum_required(VERSION 3.12)

project(
    itmoscript_interpreter
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)

if(APPLE AND CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
    set(CMAKE_APPLE_SILICON_PROCESSOR "arm64")
    set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "" FORCE)

    add_compile_options(
        -mcpu=apple-m1
        -mtune=native
        -ffast-math
        -fvectorize
        -fstrict-aliasing
    )

    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

    add_compile_options(-Oz -flto=full)
endif()

include_directories(lib)
add_subdirectory(lib)
add_subdirectory(bin)

# target_compile_definitions(itmoscript
#   PUBLIC 
#     $<$<CONFIG:Debug>:DEBUG>
# )

enable_testing()
add_subdirectory(tests)