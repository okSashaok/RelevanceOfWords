cmake_minimum_required (VERSION 3.8)
set (CMAKE_CXX_STANDARD 20)
project (RelevanceOfWords)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/program")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/program/TEST")
add_subdirectory (main)
include (FetchContent)
# nlohmann_json
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
set(JSON_BulidTests OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(nlohmann_json)
# gtest_gtest
FetchContent_Declare(
    gtest_gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)
set (gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest_gtest)
# tests
enable_testing()
add_test(NAME TestProject COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/program/TEST/TestProject.exe")