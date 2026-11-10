cmake_minimum_required(VERSION 3.20)
project(neroe VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(NEROE_BUILD_EXAMPLES "Build examples" ON): option(NEROE_BUILD_CLI "Build the neroe terminal portal" ON)
option(NEROE_BUILD_TESTS "Build tests" ON): option(NEROE_BUILD_CLI "Build the neroe terminal portal" ON)

# ---- library -------------------------------------------------------------
add_library(neroe src/neuron.cpp src/trainer.cpp src/serialization.cpp src/inspect.cpp src/card.cpp)
add_library(neroe::neroe ALIAS neroe)
target_include_directories(neroe PUBLIC include)
target_compile_features(neroe PUBLIC cxx_std_20)
target_compile_options(neroe PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>)

# ---- examples ------------------------------------------------------------
if(NEROE_BUILD_EXAMPLES)
    add_executable(logic_gates examples/logic_gates.cpp)
    add_executable(mind_portal examples/mind_portal.cpp)
    
    target_link_libraries(logic_gates PRIVATE neroe)
    target_link_libraries(mind_portal PRIVATE neroe)
endif()

# ---- terminal portal -----------------------------------------------------
if(NEROE_BUILD_CLI)
    add_executable(neroe_cli cli/neroe_cli.cpp)
    target_link_libraries(neroe_cli PRIVATE neroe)
endif()

# ---- tests ---------------------------------------------------------------
if(NEROE_BUILD_TESTS)
    enable_testing()
    add_executable(neroe_tests
        tests/test_main.cpp tests/test_neuron.cpp tests/test_random.cpp tests/test_serialization.cpp tests/test_inspect.cpp)
    target_link_libraries(neroe_tests PRIVATE neroe)
    add_test(NAME neroe_tests COMMAND neroe_tests)
endif()
