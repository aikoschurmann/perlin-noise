cmake_minimum_required(VERSION 3.14)

project(perlin-noise VERSION 0.1.0 LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(${PROJECT_NAME}_core STATIC)
target_include_directories(${PROJECT_NAME}_core PUBLIC include)
target_sources(${PROJECT_NAME}_core PRIVATE src/core.c)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE src/main.c src/input.c)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_core)

find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if(CLANG_TIDY_EXE)
    set(CMAKE_C_CLANG_TIDY ${CLANG_TIDY_EXE})
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE})
endif()

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)
endif()

include(FetchContent)
FetchContent_Declare(cfgsafe
    GIT_REPOSITORY https://github.com/aikoschurmann/cfgsafe.git
    GIT_TAG main
)
FetchContent_MakeAvailable(cfgsafe)

add_custom_target(generate_config
    COMMAND make -C ${cfgsafe_SOURCE_DIR}/tools
    COMMAND ${cfgsafe_SOURCE_DIR}/tools/out/cfg-gen ${CMAKE_SOURCE_DIR}/schemas/app.cfgsafe -o ${CMAKE_SOURCE_DIR}/include/app_config.h
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating cfgsafe headers"
)
add_dependencies(${PROJECT_NAME} generate_config)
add_dependencies(${PROJECT_NAME}_core generate_config)

# SDL2 Boilerplate
find_package(SDL2 REQUIRED CONFIG)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main SDL2::SDL2)


# Testing
enable_testing()
add_subdirectory(tests)
