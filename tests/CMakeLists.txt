include_directories(../src)

include_directories(.)

add_executable(tests
    # test sources
    rail_system_test.cpp
    # task sources
    ../src/city.h
    ../src/rail_system.cpp
    ../src/rail_system.h
    ../src/service.h
    # gtest sources
    gtest/gtest-all.cc
    gtest/gtest_main.cc
)

# add pthread for unix systems
if (UNIX)
    target_link_libraries(tests pthread)
endif ()

# for testing with coverage
#TARGET_COMPILE_OPTIONS(tests PUBLIC --coverage)
#target_link_libraries(tests gcov)
