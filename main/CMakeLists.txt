cmake_minimum_required (VERSION 3.10)
set(list
    "project/core.cpp"
    "project/function/converter_JSON.cpp"
    "project/function/auxiliary_resource/search_server.cpp"
    "project/function/auxiliary_resource/inverted_index.cpp"
    "project/function/auxiliary_resource/print_JSON.cpp"
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${main_dir}/program")
add_executable (RelevanceOfWords
    "RelevanceOfWords.cpp"
    ${list}
)
# nlohmann_json
# Google Test
target_link_libraries(RelevanceOfWords PRIVATE
    nlohmann_json::nlohmann_json
    GTest::gtest_main
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${main_dir}/program/TEST")
add_executable(TestProject
    "test.cpp"
    ${list}
)
# nlohmann_json
# Google Test
target_link_libraries(TestProject PRIVATE
    nlohmann_json::nlohmann_json
    GTest::gtest_main
)
