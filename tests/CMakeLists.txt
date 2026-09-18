add_executable(test_suite test_main.c)
target_link_libraries(test_suite PRIVATE ${PROJECT_NAME}_core)

include(FetchContent)
FetchContent_Declare(unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG master
)
FetchContent_MakeAvailable(unity)
target_link_libraries(test_suite PRIVATE unity)
add_test(NAME test_suite COMMAND test_suite)
