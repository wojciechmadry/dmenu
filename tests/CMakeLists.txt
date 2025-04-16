set(CMAKE_BUILD_TYPE Debug)

add_executable(
  drw_tests
  drw_tests.cpp
  mocks/X11/Xlib.cpp
  ../drw.cpp
  ../util.cpp
)

target_link_libraries(
  drw_tests
  GTest::gtest_main
  GTest::gmock_main
)
target_include_directories(
  drw_tests
  PUBLIC
  mocks
)

include(GoogleTest)
gtest_discover_tests(drw_tests)
