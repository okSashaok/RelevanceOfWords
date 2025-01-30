cmake_minimum_required (VERSION 3.8)
set(list
    "project/core.cpp"
    "project/function/converter_JSON.cpp"
    "project/function/auxiliary_resource/search_server.cpp"
    "project/function/auxiliary_resource/inverted_index.cpp"
    "project/function/auxiliary_resource/print_JSON.cpp"
)
add_executable (RelevanceOfWords
    "RelevanceOfWords.cpp"
    ${list}
)
add_executable(TestProject
    "test.cpp"
    ${list}
)
# nlohmann_json
# Google Test
target_link_libraries(RelevanceOfWords PRIVATE
    nlohmann_json::nlohmann_json
    GTest::gtest_main
)
target_link_libraries(TestProject PRIVATE
    nlohmann_json::nlohmann_json
    GTest::gtest_main
)
