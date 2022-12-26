add_executable(test_tokenfinder_runner
        test_tokenfinder.cpp
        )

target_include_directories(test_tokenfinder_runner
PRIVATE
${PROJECT_SOURCE_DIR}/test/include
${PROJECT_SOURCE_DIR}/include
)

custom_target_use_highest_warning_level(test_tokenfinder_runner)

add_test(
        NAME test_tokenfinder
        COMMAND test_tokenfinder_runner
)
